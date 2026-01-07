/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycastUtils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 22:59:48 by sasbai            #+#    #+#             */
/*   Updated: 2026/01/08 00:34:10 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

void	init_ray(t_cub *c, struct s_ray *r, int x)
{
	init_ray_dir(c, r, x);
	init_ray_steps(c, r);
}

void	compute_slice(t_cub *c, struct s_ray *r)
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

t_tex	*pick_tex(t_cub *c, struct s_ray *r)
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

int	fill_and_validate(t_cub *c, char **rows, int start,
			int map_h)
{
	if (fill_map_rows(c, rows, start, map_h) != 0)
		return (-1);
	if (validate_closed(c) != 0)
	{
		free_map(c);
		return (-1);
	}
	return (0);
}
