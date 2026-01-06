#include "Cub3D.h"

static t_vec	vec_add(t_vec a, t_vec b)
{
	t_vec	v;

	v.x = a.x + b.x;
	v.y = a.y + b.y;
	return (v);
}

static t_vec	vec_scale(t_vec v, double s)
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

static void	init_ray_dir(t_cub *c, struct s_ray *r, int x)
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

static void	init_ray_steps(t_cub *c, struct s_ray *r)
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

static void	init_ray(t_cub *c, struct s_ray *r, int x)
{
	init_ray_dir(c, r, x);
	init_ray_steps(c, r);
}

static void	compute_slice(t_cub *c, struct s_ray *r)
{
	r->lineh = (int)(HEIGHT / (r->perpwalldist + 1e-6));
	r->starty = -r->lineh / 2 + HEIGHT / 2;
	r->endy = r->lineh / 2 + HEIGHT / 2;
	if (r->starty < 0)
		r->starty = 0;
	if (r->endy >= HEIGHT)
		r->endy = HEIGHT - 1;
	if (r->side == 0)
		r->wallx = c->posy + r->perpwalldist * r->raydiry;
	else
		r->wallx = c->posx + r->perpwalldist * r->raydirx;
	r->wallx -= floor(r->wallx);
}

static t_tex	*pick_tex(t_cub *c, struct s_ray *r)
{
	if (r->side == 0)
	{
		if (r->raydirx < 0)
			r->texid = TEX_WE;
		else
			r->texid = TEX_EA;
	}
	else if (r->raydiry < 0)
		r->texid = TEX_NO;
	else
		r->texid = TEX_SO;
	if (r->texid >= 0 && r->texid < TEX_MAX && c->has_tex[r->texid])
		return (&c->wall[r->texid]);
	return (NULL);
}

static void	draw_tex_slice(t_cub *c, struct s_ray *r, int x, t_tex *tex)
{
	double	step;
	double	texpos;
	int	texx;
	int	texy;
	int	y;

	texx = (int)(r->wallx * tex->w);
	if ((r->side == 0 && r->raydirx > 0) || (r->side == 1 && r->raydiry < 0))
		texx = tex->w - texx - 1;
	step = (double)tex->h / (double)r->lineh;
	texpos = (r->starty - HEIGHT / 2.0 + r->lineh / 2.0) * step;
	y = r->starty;
	while (y <= r->endy)
	{
		texy = (int)texpos;
		texpos += step;
		put_px(c->frame, x, y, tex_sample_rgba(tex, texx, texy));
		y = y + 1;
	}
}

static void	draw_flat_slice(t_cub *c, struct s_ray *r, int x)
{
	unsigned int col;
	int		y;

	col = pack_rgba(180 + 30 * (r->side == 0),
		180 + 30 * (r->side == 0), 180 + 30 * (r->side == 0), 255);
	y = r->starty;
	while (y <= r->endy)
	{
		put_px(c->frame, x, y, col);
		y = y + 1;
	}
}

static void	project_and_draw(t_cub *c, struct s_ray *r, int x)
{
	t_tex	*tex;

	compute_slice(c, r);
	tex = pick_tex(c, r);
	if (tex != NULL && tex->pixels != NULL)
		draw_tex_slice(c, r, x, tex);
	else
		draw_flat_slice(c, r, x);
}

static void	cast_column(t_cub *c, int x)
{
	struct s_ray	ray;

	init_ray(c, &ray, x);
	ray.side = 0;
	while (map_at(c, ray.mapx, ray.mapy) == '0')
	{
		if (ray.sidedistx < ray.sidedisty)
		{
			ray.sidedistx += ray.deltadistx;
			ray.mapx += ray.stepx;
			ray.side = 0;
		}
		else
		{
			ray.sidedisty += ray.deltadisty;
			ray.mapy += ray.stepy;
			ray.side = 1;
		}
	}
	if (ray.side == 0)
		ray.perpwalldist = ray.sidedistx - ray.deltadistx;
	else
		ray.perpwalldist = ray.sidedisty - ray.deltadisty;
	project_and_draw(c, &ray, x);
}

void	render_frame(t_cub *c)
{
	int	x;
	int	y;
	unsigned int	col;

	y = 0;
	while (y < HEIGHT)
	{
		col = c->floor_color;
		if (y < HEIGHT / 2)
			col = c->ceil_color;
		x = 0;
		while (x < WIDTH)
		{
			put_px(c->frame, x, y, col);
			x = x + 1;
		}
		y = y + 1;
	}
	x = 0;
	while (x < WIDTH)
	{
		cast_column(c, x);
		x = x + 1;
	}
}
