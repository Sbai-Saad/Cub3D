#include "Cub3D.h"

static int	push_row(char **rows, int *row_count, char *line)
{
	rstrip(line);
	rows[*row_count] = copy_line(line);
	if (rows[*row_count] == NULL)
		return (-1);
	*row_count = *row_count + 1;
	return (0);
}

static char	**split_rows(char *file, ssize_t len, int *row_count)
{
	char	**rows;
	char	*p;
	char	*end;
	int		last;

	rows = malloc((size_t)count_lines(file, len) * sizeof(char *));
	if (rows == NULL)
		return (NULL);
	*row_count = 0;
	p = file;
	while (*p != '\0')
	{
		end = p;
		while (*end != '\0' && *end != '\n')
			end = end + 1;
		last = (*end == '\0');
		if (last == 0)
			*end = '\0';
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

static int	parse_headers(t_cub *c, char **rows, int count, int *map_start)
{
	int	tex_seen[TEX_MAX];
	int	floor_set;
	int	ceil_set;
	int	i;

	i = 0;
	while (i < TEX_MAX)
	{
		tex_seen[i] = 0;
		i = i + 1;
	}
	floor_set = 0;
	ceil_set = 0;
	i = 0;
	while (i < count && line_is_map(rows[i]) == 0)
	{
		if (parse_header_line(c, rows[i], tex_seen, &floor_set, &ceil_set) != 0)
			return (-1);
		i = i + 1;
	}
	if (i == count)
	{
		write(2, "Error: no map block\n", 20);
		return (-1);
	}
	if (tex_seen[TEX_NO] == 0 || tex_seen[TEX_SO] == 0
		|| tex_seen[TEX_WE] == 0 || tex_seen[TEX_EA] == 0)
	{
		write(2, "Error:\nmissing texture path\n", 27);
		return (-1);
	}
	if (floor_set == 0 || ceil_set == 0)
	{
		write(2, "Error:\nmissing floor/ceiling color\n", 36);
		return (-1);
	}
	*map_start = i;
	return (0);
}

static int	build_map(t_cub *c, char **rows, int count, int start)
{
	int	map_h;
	int	map_w;
	int	y;
	int	w;

	map_h = count - start;
	map_w = 0;
	y = start;
	while (y < count)
	{
		w = (int)s_len(rows[y]);
		if (w > map_w)
			map_w = w;
		y = y + 1;
	}
	if (alloc_map(c, map_h, map_w) != 0)
		return (-1);
	if (fill_map_rows(c, rows, start, map_h) != 0)
		return (-1);
	if (validate_closed(c) != 0)
	{
		free_map(c);
		return (-1);
	}
	set_plane_from_dir(c);
	return (0);
}

int	parse_map_file(t_cub *c, const char *path)
{
	ssize_t	len;
	char		*file;
	char		**rows;
	int		row_count;
	int		map_start;

	len = 0;
	if (read_all(path, &file, &len) != 0)
		return (-1);
	rows = split_rows(file, len, &row_count);
	if (rows == NULL)
	{
		free(file);
		return (-1);
	}
	if (parse_headers(c, rows, row_count, &map_start) != 0)
	{
		free_partial(rows, row_count);
		free(file);
		free_map(c);
		return (-1);
	}
	if (build_map(c, rows, row_count, map_start) != 0)
	{
		free_partial(rows, row_count);
		free(file);
		return (-1);
	}
	free_partial(rows, row_count);
	free(file);
	return (0);
}
