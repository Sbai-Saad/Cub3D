#include "Cub3D.h"

size_t	s_len(const char *s)
{
	size_t	i;

	i = 0;
	while (s && s[i])
		i = i + 1;
	return (i);
}

int	count_lines(char *file, ssize_t len)
{
	int		lines;
	ssize_t	i;

	lines = 0;
	i = 0;
	while (i < len)
	{
		if (file[i] == '\n')
			lines = lines + 1;
		i = i + 1;
	}
	if (len > 0 && file[len - 1] != '\n')
		lines = lines + 1;
	return (lines);
}

char	*copy_line(const char *s)
{
	char	*p;
	size_t	n;
	size_t	i;

	n = s_len(s);
	p = malloc(n + 1);
	if (p == NULL)
		return (NULL);
	i = 0;
	while (i < n)
	{
		p[i] = s[i];
		i = i + 1;
	}
	p[n] = '\0';
	return (p);
}

void	rstrip(char *s)
{
	size_t	n;

	n = s_len(s);
	while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r'))
	{
		s[n - 1] = '\0';
		n = n - 1;
	}
}

void	free_partial(char **rows, int count)
{
	int	i;

	if (rows == NULL)
		return ;
	i = 0;
	while (i < count)
	{
		free(rows[i]);
		i = i + 1;
	}
	free(rows);
}
