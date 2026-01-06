#include "Cub3D.h"

int	ensure_spawn(t_cub *c, char dir, int mx, int my, int *has_spawn)
{
	if (*has_spawn)
	{
		write(2, "Error:\nmultiple spawns\n", 24);
		return (-1);
	}
	*has_spawn = 1;
	c->posx = mx + 0.5;
	c->posy = my + 0.5;
	if (dir == 'N')
	{
		c->dirx = 0.0;
		c->diry = -1.0;
	}
	if (dir == 'S')
	{
		c->dirx = 0.0;
		c->diry = 1.0;
	}
	if (dir == 'E')
	{
		c->dirx = 1.0;
		c->diry = 0.0;
	}
	if (dir == 'W')
	{
		c->dirx = -1.0;
		c->diry = 0.0;
	}
	return (0);
}

void	set_plane_from_dir(t_cub *c)
{
	double	fov;

	fov = 0.66;
	c->planex = -c->diry * fov;
	c->planey = c->dirx * fov;
}
