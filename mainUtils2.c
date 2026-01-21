#include "Cub3D.h"

void	ft_exit(t_cub *cub, int code)
{
	tex_free(&cub->wall[TEX_NO]);
	tex_free(&cub->wall[TEX_SO]);
	tex_free(&cub->wall[TEX_WE]);
	tex_free(&cub->wall[TEX_EA]);
	free_map(cub);
	if (cub->frame)
		mlx_delete_image(cub->mlx, cub->frame);
	if (cub->mlx != 0)
		mlx_terminate(cub->mlx);
	exit(code);
}

void	zero_all(t_cub *cub)
{
	cub->map_path = NULL;
	ft_memset(&cub->mlx, 0, sizeof(mlx_t));
	ft_memset(cub, 0, sizeof(t_cub));
}
