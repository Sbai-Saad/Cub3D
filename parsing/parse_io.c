#include "Cub3D.h"

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

static int	append_chunk(char **buffer, ssize_t *total,
			char *chunk, ssize_t bytes)
{
	char	*temp;

	temp = malloc(*total + bytes + 1);
	if (!temp)
		return (free(*buffer), -1);
	if (*buffer)
	{
		ft_memcpy(temp, *buffer, *total);
		free(*buffer);
	}
	ft_memcpy(temp + *total, chunk, bytes);
	*total += bytes;
	*buffer = temp;
	return (0);
}

static int	load_map(int fd, char **out_buf, ssize_t *out_len)
{
	char		*buffer;
	ssize_t		total;
	ssize_t		bytes;
	char		chunk[4096];

	buffer = NULL;
	total = 0;
	while (1)
	{
		bytes = read(fd, chunk, 4096);
		if (bytes < 0)
			return (free(buffer), close(fd), -1);
		if (bytes == 0)
			break ;
		if (append_chunk(&buffer, &total, chunk, bytes) != 0)
			return (close(fd), -1);
	}
	if (!buffer)
		buffer = malloc(1);
	if (buffer)
		buffer[total] = '\0';
	*out_buf = buffer;
	if (out_len)
		*out_len = total;
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
