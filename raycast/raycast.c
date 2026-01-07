#include "Cub3D.h"

static void	draw_tex_slice(t_cub *c, struct s_ray *r, int x, t_tex *tex)
{
	double	step;
	double	texpos;
	int		texx;
	int		texy;
	int		y;

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
	unsigned int	col;
	int				y;

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
	int				x;
	int				y;
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
