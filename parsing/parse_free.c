#include "Cub3D.h"

void	free_map(t_cub *c)
{
	int	t;
	int	y;

	if (c == NULL)
		return ;
	t = 0;
	while (t < TEX_MAX)
	{
		if (c->tex_path[t] != NULL)
			free(c->tex_path[t]);
		c->tex_path[t] = NULL;
		t = t + 1;
	}
	if (c->map == NULL)
		return ;
	y = 0;
	while (y < c->map_h)
	{
		free(c->map[y]);
		y = y + 1;
	}
	free(c->map);
	c->map = NULL;
	c->map_w = 0;
	c->map_h = 0;
}

int	parse_headers(t_cub *c, char **rows, int count, int *map_start)
{
	t_header_state	st;
	int				i;

	init_header_state(&st);
	i = 0;
	while (i < count && line_is_map(rows[i]) == 0)
	{
		if (parse_header_line(c, rows[i], &st) != 0)
			return (-1);
		i = i + 1;
	}
	if (i == count)
		return (write(2, "Error: no map block\n", 20), -1);
	if (headers_missing(&st) != 0)
		return (-1);
	*map_start = i;
	return (0);
}

int	headers_missing(t_header_state *st)
{
	if (st->tex_seen[TEX_NO] == 0 || st->tex_seen[TEX_SO] == 0
		|| st->tex_seen[TEX_WE] == 0 || st->tex_seen[TEX_EA] == 0)
		return (write(2, "Error:\nmissing texture path\n", 27), -1);
	if (st->floor_set == 0 || st->ceil_set == 0)
		return (write(2, "Error:\nmissing floor/ceiling color\n", 36), -1);
	return (0);
}

void	map_dimensions(char **rows, int start, int count, t_map_dim *out)
{
	int	y;
	int	w;

	out->h = count - start;
	out->w = 0;
	y = start;
	while (y < count)
	{
		w = (int)s_len(rows[y]);
		if (w > out->w)
			out->w = w;
		y = y + 1;
	}
}

void	init_header_state(t_header_state *st)
{
	int	tex;

	if (st == NULL)
		return ;
	tex = 0;
	while (tex < TEX_MAX)
	{
		st->tex_seen[tex] = 0;
		tex = tex + 1;
	}
	st->floor_set = 0;
	st->ceil_set = 0;
}
