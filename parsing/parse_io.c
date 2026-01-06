#include "Cub3D.h"

static int	read_fail(int fd)
{
	close(fd);
	return (-1);
}

static int	open_map(const char *path)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		write(2, "Error:\nopen map\n", 17);
		return (-1);
	}
	return (fd);
}

static int	load_map(int fd, char **out_buf, ssize_t *out_len)
{
	ssize_t	size;
	ssize_t	read_bytes;

	size = lseek(fd, 0, SEEK_END);
	if (size < 0 || lseek(fd, 0, SEEK_SET) < 0)
		return (read_fail(fd));
	*out_buf = malloc((size_t)size + 1);
	if (*out_buf == NULL)
		return (read_fail(fd));
	read_bytes = read(fd, *out_buf, (size_t)size);
	if (read_bytes != size)
	{
		free(*out_buf);
		return (read_fail(fd));
	}
	(*out_buf)[size] = '\0';
	if (out_len != NULL)
		*out_len = size;
	return (0);
}

int	read_all(const char *path, char **out_buf, ssize_t *out_len)
{
	int	fd;
	int	ret;

	fd = open_map(path);
	if (fd < 0)
		return (-1);
	ret = load_map(fd, out_buf, out_len);
	close(fd);
	return (ret);
}
