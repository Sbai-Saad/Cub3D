#include "Cub3D.h"

static int	check_edges(t_cub *c, int x, int y, char ch)
{
	if (y == 0 || y == c->map_h - 1)
		if (ch == '0')
			return (write(2, "Error:\nmap not closed\n", 23), -1);
	if (x == 0 || x == c->map_w - 1)
		if (ch == '0')
			return (write(2, "Error:\nmap not closed\n", 23), -1);
	return (0);
}

static int	check_void(t_cub *c, int x, int y)
{
	if (c->map[y - 1][x] == ' ')
		return (write(2, "Error:\nmap leaks to void\n", 26), -1);
	if (c->map[y + 1][x] == ' ')
		return (write(2, "Error:\nmap leaks to void\n", 26), -1);
	if (c->map[y][x - 1] == ' ')
		return (write(2, "Error:\nmap leaks to void\n", 26), -1);
	if (c->map[y][x + 1] == ' ')
		return (write(2, "Error:\nmap leaks to void\n", 26), -1);
	return (0);
}

static int	check_cell(t_cub *c, int x, int y)
{
	char	ch;

	ch = c->map[y][x];
	if (ch != '0' && ch != '1' && ch != ' ')
		return (write(2, "Error:\ninvalid map char\n", 25), -1);
	if (check_edges(c, x, y, ch) != 0)
		return (-1);
	if (y > 0 && y < c->map_h - 1 && x > 0 && x < c->map_w - 1)
	{
		if (ch == '0' && check_void(c, x, y) != 0)
			return (-1);
	}
	return (0);
}

int	validate_closed(t_cub *c)
{
	int	x;
	int	y;

	y = 0;
	while (y < c->map_h)
	{
		x = 0;
		while (x < c->map_w)
		{
			if (check_cell(c, x, y) != 0)
				return (-1);
			x = x + 1;
		}
		y = y + 1;
	}
	return (0);
}

int	push_row(char **rows, int *row_count, char *line)
{
	rstrip(line);
	rows[*row_count] = copy_line(line);
	if (rows[*row_count] == NULL)
		return (-1);
	*row_count = *row_count + 1;
	return (0);
}
