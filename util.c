#include "Cub3D.h"

unsigned int	pack_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	return (((unsigned int)r << 24) | ((unsigned int)g << 16)
		| ((unsigned int)b << 8) | (unsigned int)a);
}

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*p;

	i = 0;
	p = (unsigned char *)s;
	while (i < n)
	{
		p[i] = (unsigned char)c;
		++i;
	}
	return (s);
}

uint32_t	tex_sample_rgba(const t_tex *t, int x, int y)
{
	size_t	idx;
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
	uint8_t	a;

	if (!t || !t->pixels || t->w == 0 || t->h == 0)
		return (0);
	if (x < 0)
		x = 0;
	if (x >= (int)t->w)
		x = (int)t->w - 1;
	if (y < 0)
		y = 0;
	if (y >= (int)t->h)
		y = (int)t->h - 1;
	idx = ((size_t)y * t->w + (size_t)x) * 4;
	r = t->pixels[idx + 0];
	g = t->pixels[idx + 1];
	b = t->pixels[idx + 2];
	a = t->pixels[idx + 3];
	return (pack_rgba(r, g, b, a));
}

void	put_px(mlx_image_t *img, int x, int y, unsigned int col)
{
	if (!img)
		return ;
	if ((unsigned int)x < img->width && (unsigned int)y < img->height)
		mlx_put_pixel(img, (uint32_t)x, (uint32_t)y, col);
}
