#include "Cub3D.h"

void	free_map(t_cub *c)
{
	int	t;
	int	y;

	if (c == NULL)
		return ;
	t = 0;
	while (t < TEX_MAX)
	{
		if (c->tex_path[t] != NULL)
			free(c->tex_path[t]);
		c->tex_path[t] = NULL;
		t = t + 1;
	}
	if (c->map == NULL)
		return ;
	y = 0;
	while (y < c->map_h)
	{
		free(c->map[y]);
		y = y + 1;
	}
	free(c->map);
	c->map = NULL;
	c->map_w = 0;
	c->map_h = 0;
}
