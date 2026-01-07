#include "Cub3D.h"

static int	build_map(t_cub *c, char **rows, int count, int start)
{
	t_map_dim	dims;

	map_dimensions(rows, start, count, &dims);
	if (alloc_map(c, dims.h, dims.w) != 0)
		return (-1);
	if (fill_and_validate(c, rows, start, dims.h) != 0)
		return (-1);
	set_plane_from_dir(c);
	return (0);
}

static int	read_rows(const char *path, char **file, ssize_t *len,
			t_rows_result *out)
{
	*len = 0;
	if (read_all(path, file, len) != 0)
		return (-1);
	out->rows = split_rows(*file, *len, &out->row_count);
	if (out->rows == NULL)
	{
		free(*file);
		return (-1);
	}
	return (0);
}

static int	validate_extension(const char *path)
{
	size_t	len;

	len = s_len(path);
	if (len < 4 || path[len - 4] != '.' || path[len - 3] != 'c'
		|| path[len - 2] != 'u' || path[len - 1] != 'b')
		return (write(2, "Error:\nmap must end with .cub\n", 30), -1);
	return (0);
}

static int	parse_sections(t_cub *c, char **rows, int row_count)
{
	int	map_start;

	if (parse_headers(c, rows, row_count, &map_start) != 0)
	{
		free_partial(rows, row_count);
		free_map(c);
		return (-1);
	}
	if (build_map(c, rows, row_count, map_start) != 0)
	{
		free_partial(rows, row_count);
		return (-1);
	}
	return (0);
}

int	parse_map_file(t_cub *c, const char *path)
{
	ssize_t			len;
	char			*file;
	t_rows_result	rows;

	if (validate_extension(path) != 0)
		return (-1);
	if (read_rows(path, &file, &len, &rows) != 0)
		return (-1);
	if (parse_sections(c, rows.rows, rows.row_count) != 0)
	{
		free(file);
		return (-1);
	}
	free_partial(rows.rows, rows.row_count);
	free(file);
	return (0);
}
