#include "Cub3D.h"

int	alloc_map(t_cub *c, int map_h, int map_w)
{
	int	y;

	c->map_h = map_h;
	c->map_w = map_w;
	c->map = malloc((size_t)map_h * sizeof(char *));
	if (c->map == NULL)
		return (write(2, "Error:\nalloc map rows\n", 22), -1);
	y = 0;
	while (y < map_h)
	{
		c->map[y] = malloc((size_t)map_w + 1);
		if (c->map[y] == NULL)
		{
			write(2, "Error:\nalloc map row\n", 21);
			free_map(c);
			return (-1);
		}
		ft_memset(c->map[y], ' ', (size_t)map_w);
		c->map[y][map_w] = '\0';
		y = y + 1;
	}
	return (0);
}

static int	set_cell(t_cub *c, char ch, int x, int y, int *has_spawn)
{
	if (ch == '\t')
		ch = ' ';
	if (ch == 'N' || ch == 'S' || ch == 'E' || ch == 'W')
	{
		if (ensure_spawn(c, ch, x, y, has_spawn) != 0)
			return (-1);
		ch = '0';
	}
	c->map[y][x] = ch;
	return (0);
}

int	fill_map_rows(t_cub *c, char **rows, int start, int map_h)
{
	int	y;
	int	x;
	int	w;
	int	has_spawn;

	has_spawn = 0;
	y = 0;
	while (y < map_h)
	{
		w = (int)s_len(rows[start + y]);
		x = 0;
		while (x < w)
		{
			if (set_cell(c, rows[start + y][x], x, y, &has_spawn) != 0)
				return (-1);
			x = x + 1;
		}
		y = y + 1;
	}
	if (has_spawn == 0)
	{
		write(2, "Error:\nno spawn\n", 17);
		free_map(c);
		return (-1);
	}
	return (0);
}
