#include "Cub3D.h"

static size_t s_len(const char *s) {
    size_t i = 0;
    while (s && s[i]) ++i;
    return i;
}

static char *copy_line(const char *s)
{
    size_t n = s_len(s);
    char *p = malloc(n + 1);
    if (!p) return NULL;
    size_t i = 0;
    while (i < n) { p[i] = s[i]; ++i; }
    p[n] = '\0';
    return p;
}

static char *read_all(const char *path, ssize_t *out_len) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        write(2, "Error:\nopen map\n", 17);
        return NULL;
    }
    size_t cap = 4096;
    size_t len = 0;
    char *buf = malloc(cap + 1);
    if (!buf) {
        close(fd);
        return NULL;
    }
    while (1) {
        if (len + 2048 > cap) {
            size_t ncap = cap * 2;
            char *nbuf = malloc(ncap + 1);
            if (!nbuf) { free(buf); close(fd); return NULL; }
            size_t k = 0;
            while (k < len) { nbuf[k] = buf[k]; ++k; }
            free(buf);
            buf = nbuf;
            cap = ncap;
        }
        ssize_t r = read(fd, buf + len, (ssize_t)(cap - len));
        if (r < 0) {
            free(buf);
            close(fd);
            return NULL;
        }
        if (r == 0) break;
        len += (size_t)r;
    }
    close(fd);
    buf[len] = '\0';
    if (out_len) *out_len = (ssize_t)len;
    return buf;
}

static void rstrip(char *s) {
    size_t n = s_len(s);
    while (n && (s[n-1] == '\n' || s[n-1] == '\r')) {
        s[n-1] = '\0';
        --n;
    }
}

static int ensure_spawn(t_cub *c, char dir, int mx, int my, int *has_spawn) {
    if (*has_spawn) {
        write(2, "Error:\nmultiple spawns\n", 24);
        return -1;
    }
    *has_spawn = 1;
    c->posX = mx + 0.5;
    c->posY = my + 0.5;
    if (dir == 'N') { c->dirX = 0.0;  c->dirY = -1.0; }
    if (dir == 'S') { c->dirX = 0.0;  c->dirY =  1.0; }
    if (dir == 'E') { c->dirX = 1.0;  c->dirY =  0.0; }
    if (dir == 'W') { c->dirX = -1.0; c->dirY =  0.0; }
    return 0;
}

static void set_plane_from_dir(t_cub *c) {
    double fov = 0.66;
    c->planeX = -c->dirY * fov;
    c->planeY =  c->dirX * fov;
}

static void free_partial(char **rows, int count) {
    if (!rows) return;
    int i = 0;
    while (i < count) {
        free(rows[i]);
        ++i;
    }
    free(rows);
}

static int is_map_char(char ch) {
    if (ch == '0' || ch == '1' || ch == ' ' || ch == 'N' || ch == 'S' || ch == 'E' || ch == 'W') return 1;
    if (ch == '\t') return 1;
    return 0;
}

static int parse_rgb(const char *s, unsigned int *out_rgba) {
    int vals[3];
    int vi = 0;
    size_t i = 0;
    while (vi < 3) {
        while (s[i] == ' ' || s[i] == '\t') ++i;
        if (s[i] < '0' || s[i] > '9') return -1;
        int v = 0;
        while (s[i] >= '0' && s[i] <= '9') {
            v = v * 10 + (s[i] - '0');
            ++i;
        }
        if (v < 0 || v > 255) return -1;
        vals[vi++] = v;
        while (s[i] == ' ' || s[i] == '\t') ++i;
        if (vi < 3) {
            if (s[i] != ',') return -1;
            ++i;
        }
    }
    while (s[i] == ' ' || s[i] == '\t') ++i;
    if (s[i] != '\0') return -1;
    *out_rgba = pack_rgba((uint8_t)vals[0], (uint8_t)vals[1], (uint8_t)vals[2], 255);
    return 0;
}

static int parse_header_line(t_cub *c, const char *line, int *tex_seen, int *floor_set, int *ceil_set) {
    const char *p = line;
    while (*p == ' ' || *p == '\t') ++p;
    if (*p == '\0') return 0; // skip blank

    if ((p[0] == 'N' && p[1] == 'O') || (p[0] == 'S' && p[1] == 'O') || (p[0] == 'W' && p[1] == 'E') || (p[0] == 'E' && p[1] == 'A')) {
        int id;
        if (p[0] == 'N') id = TEX_NO;
        else if (p[0] == 'S') id = TEX_SO;
        else if (p[0] == 'W') id = TEX_WE;
        else id = TEX_EA;
        p += 2;
        while (*p == ' ' || *p == '\t') ++p;
        if (*p == '\0') { write(2, "Error:\nempty texture path\n", 27); return -1; }
        if (tex_seen[id]) { write(2, "Error:\nduplicate texture\n", 25); return -1; }
        c->tex_path[id] = copy_line(p);
        if (!c->tex_path[id]) { write(2, "Error:\nalloc texture path\n", 27); return -1; }
        tex_seen[id] = 1;
        return 0;
    }

    if (p[0] == 'F' && (p[1] == ' ' || p[1] == '\t')) {
        if (*floor_set) { write(2, "Error:\nduplicate floor color\n", sizeof("Error:\nduplicate floor color\n") - 1); return -1; }
        if (parse_rgb(p + 1, &c->floor_color) != 0) { write(2, "Error:\ninvalid floor color\n", sizeof("Error:\ninvalid floor color\n") - 1); return -1; }
        *floor_set = 1;
        return 0;
    }

    if (p[0] == 'C' && (p[1] == ' ' || p[1] == '\t')) {
        if (*ceil_set) { write(2, "Error:\nduplicate ceiling color\n", sizeof("Error:\nduplicate ceiling color\n") - 1); return -1; }
        if (parse_rgb(p + 1, &c->ceil_color) != 0) { write(2, "Error:\ninvalid ceiling color\n", sizeof("Error:\ninvalid ceiling color\n") - 1); return -1; }
        *ceil_set = 1;
        return 0;
    }

    write(2, "Error:\ninvalid header line\n", 27);
    return -1;
}

static int validate_closed(t_cub *c) {
    int y = 0;
    while (y < c->map_h) {
        int x = 0;
        while (x < c->map_w) {
            char ch = c->map[y][x];
            if (ch != '0' && ch != '1' && ch != ' ') {
                write(2, "Error:\ninvalid map char\n", 25);
                return -1;
            }
            if (y == 0 || y == c->map_h - 1 || x == 0 || x == c->map_w - 1) {
                if (ch == '0') {
                    write(2, "Error:\nmap not closed\n", 23);
                    return -1;
                }
            } else if (ch == '0') {
                if (c->map[y-1][x] == ' ' || c->map[y+1][x] == ' ' || c->map[y][x-1] == ' ' || c->map[y][x+1] == ' ') {
                    write(2, "Error:\nmap leaks to void\n", 26);
                    return -1;
                }
            }
            ++x;
        }
        ++y;
    }
    return 0;
}

static int line_is_map(const char *s) {
    if (!s) return 0;
    size_t i = 0;
    int has_tile = 0;
    while (s[i]) {
        if (!is_map_char(s[i])) return 0;
        if (s[i] == '0' || s[i] == '1' || s[i] == 'N' || s[i] == 'S' || s[i] == 'E' || s[i] == 'W') {
            has_tile = 1;
        }
        ++i;
    }
    return has_tile;
}

int parse_map_file(t_cub *c, const char *path) {
    ssize_t len = 0;
    char *file = read_all(path, &len);
    if (!file) return -1;

    // Count lines
    int lines = 0;
    ssize_t i = 0;
    while (i < len) {
        if (file[i] == '\n') ++lines;
        ++i;
    }
    if (len > 0 && file[len-1] != '\n') ++lines;
    if (lines == 0) {
        write(2, "Error:\nempty file\n", 19);
        free(file);
        return -1;
    }

    char **rows = malloc((size_t)lines * sizeof(char*));
    if (!rows) {
        write(2, "Error:\nalloc rows\n", 18);
        free(file);
        return -1;
    }

    int row = 0;
    char *p = file;
    while (*p) {
        char *end = p;
        while (*end && *end != '\n') ++end;
        int is_last = (*end == '\0');
        if (!is_last) *end = '\0';
        rstrip(p);
        int w = (int)s_len(p);
        rows[row++] = copy_line(p);
        if (!rows[row-1]) {
            write(2, "Error:\nalloc line\n", 18);
            free(file); free_partial(rows, row-1); return -1;
        }
        if (is_last) break;
        p = end + 1;
    }

    int tex_seen[TEX_MAX] = {0,0,0,0};
    int floor_set = 0, ceil_set = 0;

    int map_start = 0;
    while (map_start < row && !line_is_map(rows[map_start])) {
        if (parse_header_line(c, rows[map_start], tex_seen, &floor_set, &ceil_set) != 0) {
            free(file); free_partial(rows, row); free_map(c); return -1;
        }
        ++map_start;
    }
    if (map_start == row) {
        write(2, "Error: no map block\n", 20);
        free(file); free_partial(rows, row); free_map(c); return -1;
    }

    int map_end = map_start;
    while (map_end < row) {
        if (!line_is_map(rows[map_end])) {
            write(2, "Error:\ninvalid map line\n", 24);
            free(file); free_partial(rows, row); free_map(c); return -1;
        }
        ++map_end;
    }

    if (!tex_seen[TEX_NO] || !tex_seen[TEX_SO] || !tex_seen[TEX_WE] || !tex_seen[TEX_EA]) {
        write(2, "Error:\nmissing texture path\n", 27);
        free(file); free_partial(rows, row); free_map(c); return -1;
    }
    if (!floor_set || !ceil_set) {
        write(2, "Error:\nmissing floor/ceiling color\n", 36);
        free(file); free_partial(rows, row); free_map(c); return -1;
    }

    int map_maxw = 0;
    int mi = map_start;
    while (mi < row) {
        int w = (int)s_len(rows[mi]);
        if (w > map_maxw) map_maxw = w;
        ++mi;
    }

    c->map_h = row - map_start;
    c->map_w = map_maxw;
    c->map = malloc((size_t)c->map_h * sizeof(char*));
    if (!c->map) {
        write(2, "Error:\nalloc map rows\n", 22);
        free(file); free_partial(rows, row); return -1;
    }

    int has_spawn = 0;
    int y = 0;
    while (y < c->map_h) {
        c->map[y] = malloc((size_t)c->map_w + 1);
        if (!c->map[y]) {
            write(2, "Error:\nalloc map row\n", 22);
            free(file); free_partial(rows, row); free_map(c); return -1; }
        ft_memset(c->map[y], ' ', (size_t)c->map_w);
        c->map[y][c->map_w] = '\0';
        int w = (int)s_len(rows[map_start + y]);
        int x = 0;
        while (x < w) {
            char ch = rows[map_start + y][x];
            if (ch == '\t') ch = ' ';
            if (ch == 'N' || ch == 'S' || ch == 'E' || ch == 'W') {
                if (ensure_spawn(c, ch, x, y, &has_spawn) != 0) {
                    free(file); free_partial(rows, row); free_map(c); return -1;
                }
                ch = '0';
            }
            c->map[y][x] = ch;
            ++x;
        }
        ++y;
    }

    free_partial(rows, row);
    free(file);

    if (!has_spawn) {
        write(2, "Error:\nno spawn\n", 17);
        free_map(c);
        return -1;
    }

    if (validate_closed(c) != 0) {
        free_map(c);
        return -1;
    }

    set_plane_from_dir(c);

    return 0;
}

void free_map(t_cub *c) {
    if (!c) return;
    int t = 0;
    while (t < TEX_MAX) {
        if (c->tex_path[t]) free(c->tex_path[t]);
        c->tex_path[t] = NULL;
        ++t;
    }
    if (!c->map) return;
    int y = 0;
    while (y < c->map_h) {
        free(c->map[y]);
        ++y;
    }
    free(c->map);
    c->map = NULL;
    c->map_w = 0;
    c->map_h = 0;
}
