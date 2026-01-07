#include "Cub3D.h"

t_vec	vec_add(t_vec a, t_vec b)
{
	t_vec	v;

	v.x = a.x + b.x;
	v.y = a.y + b.y;
	return (v);
}

t_vec	vec_scale(t_vec v, double s)
{
	t_vec	out;

	out.x = v.x * s;
	out.y = v.y * s;
	return (out);
}

char	map_at(t_cub *c, int x, int y)
{
	if (c->map == NULL)
		return ('1');
	if (x < 0 || y < 0 || x >= c->map_w || y >= c->map_h)
		return ('1');
	return (c->map[y][x]);
}

void	init_ray_dir(t_cub *c, struct s_ray *r, int x)
{
	double	camera;
	t_vec	raydir;
	t_vec	delta;

	camera = 2.0 * x / (double)WIDTH - 1.0;
	raydir = vec_add((t_vec){c->dirx, c->diry},
			vec_scale((t_vec){c->planex, c->planey}, camera));
	r->raydirx = raydir.x;
	r->raydiry = raydir.y;
	r->mapx = (int)c->posx;
	r->mapy = (int)c->posy;
	delta.x = 1e30;
	delta.y = 1e30;
	if (raydir.x != 0.0)
		delta.x = fabs(1.0 / raydir.x);
	if (raydir.y != 0.0)
		delta.y = fabs(1.0 / raydir.y);
	r->deltadistx = delta.x;
	r->deltadisty = delta.y;
}

void	init_ray_steps(t_cub *c, struct s_ray *r)
{
	if (r->raydirx < 0)
	{
		r->stepx = -1;
		r->sidedistx = (c->posx - r->mapx) * r->deltadistx;
	}
	else
	{
		r->stepx = 1;
		r->sidedistx = (r->mapx + 1.0 - c->posx) * r->deltadistx;
	}
	if (r->raydiry < 0)
	{
		r->stepy = -1;
		r->sidedisty = (c->posy - r->mapy) * r->deltadisty;
	}
	else
	{
		r->stepy = 1;
		r->sidedisty = (r->mapy + 1.0 - c->posy) * r->deltadisty;
	}
}
