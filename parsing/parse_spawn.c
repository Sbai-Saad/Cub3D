#include "Cub3D.h"

static void	set_dir_from_char(t_cub *c, char dir)
{
	if (dir == 'N')
	{
		c->dirx = 0.0;
		c->diry = -1.0;
	}
	else if (dir == 'S')
	{
		c->dirx = 0.0;
		c->diry = 1.0;
	}
	else if (dir == 'E')
	{
		c->dirx = 1.0;
		c->diry = 0.0;
	}
	else if (dir == 'W')
	{
		c->dirx = -1.0;
		c->diry = 0.0;
	}
}

int	ensure_spawn(t_cub *c, char dir, t_pos m, int *has_spawn)
{
	if (*has_spawn)
	{
		write(2, "Error:\nmultiple spawns\n", 24);
		return (-1);
	}
	*has_spawn = 1;
	c->posx = m.x + 0.5;
	c->posy = m.y + 0.5;
	set_dir_from_char(c, dir);
	return (0);
}

void	set_plane_from_dir(t_cub *c)
{
	double	fov;

	fov = 0.66;
	c->planex = -c->diry * fov;
	c->planey = c->dirx * fov;
}

char	*consume_line(char *p, int *last)
{
	char	*end;

	end = p;
	while (*end != '\0' && *end != '\n')
		end = end + 1;
	*last = (*end == '\0');
	if (*last == 0)
		*end = '\0';
	return (end);
}

int	alloc_rows(char ***rows, char *file, ssize_t len)
{
	*rows = malloc((size_t)count_lines(file, len) * sizeof(char *));
	if (*rows == NULL)
		return (-1);
	return (0);
}
