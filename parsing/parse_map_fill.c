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

static int	set_cell(t_cub *c, char ch, t_pos p, int *has_spawn)
{
	if (ch == '\t')
		ch = ' ';
	if (ch == 'N' || ch == 'S' || ch == 'E' || ch == 'W')
	{
		if (ensure_spawn(c, ch, p, has_spawn) != 0)
			return (-1);
		ch = '0';
	}
	c->map[p.y][p.x] = ch;
	return (0);
}

static int	fill_row(t_cub *c, t_row_ctx ctx, int *has_spawn)
{
	int	x;
	int	w;

	w = (int)s_len(ctx.rows[ctx.start + ctx.y]);
	x = 0;
	while (x < w)
	{
		if (set_cell(c, ctx.rows[ctx.start + ctx.y][x],
			(t_pos){x, ctx.y}, has_spawn) != 0)
			return (-1);
		x = x + 1;
	}
	return (0);
}

int	fill_map_rows(t_cub *c, char **rows, int start, int map_h)
{
	int	y;
	int	has_spawn;

	has_spawn = 0;
	y = 0;
	while (y < map_h)
	{
		if (fill_row(c, (t_row_ctx){rows, start, y}, &has_spawn) != 0)
			return (-1);
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

char	**split_rows(char *file, ssize_t len, int *row_count)
{
	char	**rows;
	char	*p;
	char	*end;
	int		last;

	if (alloc_rows(&rows, file, len) != 0)
		return (NULL);
	*row_count = 0;
	p = file;
	while (*p != '\0')
	{
		end = consume_line(p, &last);
		if (push_row(rows, row_count, p) != 0)
		{
			free_partial(rows, *row_count);
			return (NULL);
		}
		if (last)
			break ;
		p = end + 1;
	}
	return (rows);
}
