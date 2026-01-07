#include "Cub3D.h"

static void	skip_spaces(const char **s)
{
	while (**s == ' ' || **s == '\t')
		*s = *s + 1;
}

static int	read_val(const char **s, int *out)
{
	*out = 0;
	if (**s < '0' || **s > '9')
		return (-1);
	while (**s >= '0' && **s <= '9')
	{
		*out = *out * 10 + (**s - '0');
		*s = *s + 1;
	}
	if (*out < 0 || *out > 255)
		return (-1);
	return (0);
}

int	parse_rgb(const char *s, unsigned int *out_rgba)
{
	int	vals[3];
	int	idx;

	idx = 0;
	while (idx < 3)
	{
		skip_spaces(&s);
		if (read_val(&s, &vals[idx]) != 0)
			return (-1);
		skip_spaces(&s);
		if (idx < 2)
		{
			if (*s != ',')
				return (-1);
			s = s + 1;
		}
		idx = idx + 1;
	}
	if (*s != '\0')
		return (-1);
	*out_rgba = pack_rgba((uint8_t)vals[0], (uint8_t)vals[1],
			(uint8_t)vals[2], 255);
	return (0);
}

void	error_exit(void)
{
	exit(EXIT_FAILURE);
}

void	try_move(t_cub *cub, double dx, double dy)
{
	double	nx;
	double	ny;

	nx = cub->posx + dx;
	ny = cub->posy + dy;
	if (map_at(cub, (int)nx, (int)cub->posy) == '0')
		cub->posx = nx;
	if (map_at(cub, (int)cub->posx, (int)ny) == '0')
		cub->posy = ny;
}
