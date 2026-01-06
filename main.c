#include "Cub3D.h"

static void error_exit(void) {
    exit(EXIT_FAILURE);
}

static void try_move(t_cub* c, double dx, double dy) {
    double nx = c->posX + dx;
    double ny = c->posY + dy;
    if (map_at(c, (int)nx, (int)c->posY) == '0') c->posX = nx;
    if (map_at(c, (int)c->posX, (int)ny) == '0') c->posY = ny;
}

static void rotate_player(t_cub* c, double angle) {
    double oldDirX = c->dirX;
    c->dirX = c->dirX * cos(angle) - c->dirY * sin(angle);
    c->dirY = oldDirX * sin(angle) + c->dirY * cos(angle);

    double oldPlaneX = c->planeX;
    c->planeX = c->planeX * cos(angle) - c->planeY * sin(angle);
    c->planeY = oldPlaneX * sin(angle) + c->planeY * cos(angle);
}

static void init_game(t_cub* c, const char* map_path) {
    ft_memset(c, 0, sizeof(*c));

    c->ceil_color  = pack_rgba(135,206,235,255);
    c->floor_color = pack_rgba(40,40,40,255);

    if (!map_path) {
        write(2, "Error: no map path\n", 20);
        error_exit();
    }
    if (parse_map_file(c, map_path) != 0) {
        error_exit();
    }

    c->mlx = mlx_init(WIDTH, HEIGHT, "KYOB TRWADI", true);
    if (!c->mlx) {
        write(2, "Error:\nmlx_init failed\n", 23);
        exit(EXIT_FAILURE);
    }

    c->frame = mlx_new_image(c->mlx, WIDTH, HEIGHT);
    if (!c->frame) {
        write(2, "Error:\nmlx_new_image failed\n", 28);
        exit(EXIT_FAILURE);
    }
    if (mlx_image_to_window(c->mlx, c->frame, 0, 0) < 0) {
        write(2, "Error:\nmlx_image_to_window failed\n", 35);
        exit(EXIT_FAILURE);
    }

    const char *paths[TEX_MAX] = {
        c->tex_path[TEX_NO],
        c->tex_path[TEX_SO],
        c->tex_path[TEX_WE],
        c->tex_path[TEX_EA]
    };
    c->has_tex[TEX_NO] = paths[TEX_NO] && tex_load_xpm42(&c->wall[TEX_NO], paths[TEX_NO]);
    c->has_tex[TEX_SO] = paths[TEX_SO] && tex_load_xpm42(&c->wall[TEX_SO], paths[TEX_SO]);
    c->has_tex[TEX_WE] = paths[TEX_WE] && tex_load_xpm42(&c->wall[TEX_WE], paths[TEX_WE]);
    c->has_tex[TEX_EA] = paths[TEX_EA] && tex_load_xpm42(&c->wall[TEX_EA], paths[TEX_EA]);
    if (!c->has_tex[TEX_NO] || !c->has_tex[TEX_SO] || !c->has_tex[TEX_WE] || !c->has_tex[TEX_EA]) {
        write(2, "Error:\nload texture\n", 21);
        error_exit();
    }
}

static void on_key(mlx_key_data_t kd, void* param) {
    t_cub* c = (t_cub*)param;
    if (kd.key == MLX_KEY_ESCAPE && kd.action == MLX_PRESS) {
        mlx_close_window(c->mlx);
    }
}

static void on_loop(void* param) {
    t_cub* c = (t_cub*)param;
    double dt = c->mlx->delta_time;
    double ms = MOVE_S * dt;
    double rs = ROT_S * dt;

    if (mlx_is_key_down(c->mlx, MLX_KEY_W)) try_move(c,  c->dirX * ms,  c->dirY * ms);
    if (mlx_is_key_down(c->mlx, MLX_KEY_S)) try_move(c, -c->dirX * ms, -c->dirY * ms);

    double leftX = -c->dirY;
    double leftY =  c->dirX;
    if (mlx_is_key_down(c->mlx, MLX_KEY_A)) try_move(c,  leftX * ms,  leftY * ms);
    if (mlx_is_key_down(c->mlx, MLX_KEY_D)) try_move(c, -leftX * ms, -leftY * ms);

    if (mlx_is_key_down(c->mlx, MLX_KEY_LEFT))  rotate_player(c,  rs);
    if (mlx_is_key_down(c->mlx, MLX_KEY_RIGHT)) rotate_player(c, -rs);

    render_frame(c);
}

int main(int argc, char** argv) {
    const char* map_path = NULL;
    if (argc > 1) {
        map_path = argv[1];
    }

    t_cub c;
    init_game(&c, map_path);

    mlx_key_hook(c.mlx, on_key, &c);
    mlx_loop_hook(c.mlx, on_loop, &c);
    render_frame(&c);

    mlx_loop(c.mlx);

    int i = 0;
    while (i < TEX_MAX) {
        tex_free(&c.wall[i]);
        ++i;
    }
    free_map(&c);
    if (c.frame) mlx_delete_image(c.mlx, c.frame);
    mlx_terminate(c.mlx);
    return 0;
}