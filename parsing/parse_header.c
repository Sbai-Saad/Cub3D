#include "Cub3D.h"

int	is_map_char(char ch)
{
	if (ch == '0' || ch == '1' || ch == ' ')
		return (1);
	if (ch == 'N' || ch == 'S' || ch == 'E' || ch == 'W')
		return (1);
	if (ch == '\t')
		return (1);
	return (0);
}

static int	handle_texture(t_cub *c, const char *p, int *tex_seen)
{
	int	id;

	id = TEX_NO;
	if (p[0] == 'S')
		id = TEX_SO;
	if (p[0] == 'W')
		id = TEX_WE;
	if (p[0] == 'E')
		id = TEX_EA;
	p = p + 2;
	while (*p == ' ' || *p == '\t')
		p = p + 1;
	if (*p == '\0')
		return (write(2, "Error:\nempty texture path\n", 27), -1);
	if (tex_seen[id])
		return (write(2, "Error:\nduplicate texture\n", 25), -1);
	c->tex_path[id] = copy_line(p);
	if (c->tex_path[id] == NULL)
		return (write(2, "Error:\nalloc texture path\n", 27), -1);
	tex_seen[id] = 1;
	return (0);
}

static int	handle_floor(t_cub *c, const char *p, int *floor_set)
{
	if (*floor_set)
		return (write(2, "Error:\nduplicate floor color\n", 29), -1);
	if (parse_rgb(p + 1, &c->floor_color) != 0)
		return (write(2, "Error:\ninvalid floor color\n", 27), -1);
	*floor_set = 1;
	return (0);
}

static int	handle_ceiling(t_cub *c, const char *p, int *ceil_set)
{
	if (*ceil_set)
		return (write(2, "Error:\nduplicate ceiling color\n", 31), -1);
	if (parse_rgb(p + 1, &c->ceil_color) != 0)
		return (write(2, "Error:\ninvalid ceiling color\n", 30), -1);
	*ceil_set = 1;
	return (0);
}

int	parse_header_line(t_cub *c, const char *line, int *tex_seen,
	int *floor_set, int *ceil_set)
{
	const char	*p;

	p = line;
	while (*p == ' ' || *p == '\t')
		p = p + 1;
	if (*p == '\0')
		return (0);
	if ((p[0] == 'N' && p[1] == 'O') || (p[0] == 'S' && p[1] == 'O')
		|| (p[0] == 'W' && p[1] == 'E') || (p[0] == 'E' && p[1] == 'A'))
		return (handle_texture(c, p, tex_seen));
	if (p[0] == 'F' && (p[1] == ' ' || p[1] == '\t'))
		return (handle_floor(c, p, floor_set));
	if (p[0] == 'C' && (p[1] == ' ' || p[1] == '\t'))
		return (handle_ceiling(c, p, ceil_set));
	write(2, "Error:\ninvalid header line\n", 27);
	return (-1);
}

int	line_is_map(const char *s)
{
	size_t	i;
	int		flag;

	if (s == NULL)
		return (0);
	i = 0;
	flag = 0;
	while (s[i] != '\0')
	{
		if (is_map_char(s[i]) == 0)
			return (0);
		if (s[i] == '0' || s[i] == '1' || s[i] == 'N'
			|| s[i] == 'S' || s[i] == 'E' || s[i] == 'W')
			flag = 1;
		i = i + 1;
	}
	return (flag);
}
