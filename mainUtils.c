#include "Cub3D.h"

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

void	rotate_player(t_cub *cub, double angle)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = cub->dirx;
	cub->dirx = cub->dirx * cos(angle) - cub->diry * sin(angle);
	cub->diry = old_dir_x * sin(angle) + cub->diry * cos(angle);
	old_plane_x = cub->planex;
	cub->planex = cub->planex * cos(angle) - cub->planey * sin(angle);
	cub->planey = old_plane_x * sin(angle) + cub->planey * cos(angle);
}

int	load_textures(t_cub *cub)
{
	if (!cub->tex_path[TEX_NO] || !cub->tex_path[TEX_SO]
		|| !cub->tex_path[TEX_WE] || !cub->tex_path[TEX_EA])
		return (1);
	cub->has_tex[TEX_NO] = tex_load_xpm42(&cub->wall[TEX_NO],
			cub->tex_path[TEX_NO]);
	cub->has_tex[TEX_SO] = tex_load_xpm42(&cub->wall[TEX_SO],
			cub->tex_path[TEX_SO]);
	cub->has_tex[TEX_WE] = tex_load_xpm42(&cub->wall[TEX_WE],
			cub->tex_path[TEX_WE]);
	cub->has_tex[TEX_EA] = tex_load_xpm42(&cub->wall[TEX_EA],
			cub->tex_path[TEX_EA]);
	if (!cub->has_tex[TEX_NO] || !cub->has_tex[TEX_SO]
		|| !cub->has_tex[TEX_WE] || !cub->has_tex[TEX_EA])
		return (1);
	return (0);
}

int	init_game(t_cub *cub, const char *map_path)
{
	int	ret;

	if (!map_path)
	{
		write(2, "Error:\nno map path\n", 20);
		return (1);
	}
	ft_memset(cub, 0, sizeof(*cub));
	cub->ceil_color = pack_rgba(135, 206, 235, 255);
	cub->floor_color = pack_rgba(40, 40, 40, 255);
	ret = parse_map_file(cub, map_path);
	if (ret != 0)
		return (1);
	cub->mlx = mlx_init(WIDTH, HEIGHT, "KYOB TROI DI", true);
	if (!cub->mlx)
		return (1);
	cub->frame = mlx_new_image(cub->mlx, WIDTH, HEIGHT);
	if (!cub->frame)
		return (1);
	if (mlx_image_to_window(cub->mlx, cub->frame, 0, 0) < 0)
		return (1);
	if (load_textures(cub) != 0)
	{
		write(2, "Error:\nload texture\n", 21);
		return (1);
	}
	return (0);
}
