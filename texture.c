/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasbai <sasbai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 22:59:28 by sasbai            #+#    #+#             */
/*   Updated: 2026/01/07 22:59:29 by sasbai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

static void	tex_prepare_from_xpm(t_tex *t, xpm_t *xpm)
{
	t->handle = (mlx_texture_t *)&xpm->texture;
	t->w = xpm->texture.width;
	t->h = xpm->texture.height;
}

static bool	tex_copy_pixels(t_tex *t, xpm_t *xpm)
{
	size_t		sz;
	uint8_t		*buf;
	size_t		i;

	sz = (size_t)t->w * t->h * 4;
	buf = malloc(sz);
	if (!buf)
	{
		t->handle = NULL;
		t->pixels = NULL;
		t->w = 0;
		t->h = 0;
		return (false);
	}
	i = 0;
	while (i < sz)
	{
		buf[i] = xpm->texture.pixels[i];
		++i;
	}
	t->handle = NULL;
	t->pixels = buf;
	return (true);
}

bool	tex_load_xpm42(t_tex *t, const char *path)
{
	xpm_t	*xpm;

	if (!t || !path)
		return (false);
	xpm = mlx_load_xpm42(path);
	if (!xpm)
		return (false);
	tex_prepare_from_xpm(t, xpm);
	if (!tex_copy_pixels(t, xpm))
	{
		mlx_delete_xpm42(xpm);
		return (false);
	}
	mlx_delete_xpm42(xpm);
	return (true);
}

void	tex_free(t_tex *t)
{
	if (!t)
		return ;
	if (!t->handle && t->pixels)
		free(t->pixels);
	if (t->handle)
		mlx_delete_texture(t->handle);
	t->handle = NULL;
	t->pixels = NULL;
	t->w = 0;
	t->h = 0;
}
