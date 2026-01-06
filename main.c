#include "Cub3D.h"

static void	on_key(mlx_key_data_t key, void *param)
{
	t_cub	*cub;

	cub = (t_cub *)param;
	if (key.key == MLX_KEY_ESCAPE && key.action == MLX_PRESS)
		mlx_close_window(cub->mlx);
}

static void	update_move(t_cub *cub, double ms)
{
	double	left_x;
	double	left_y;

	left_x = -cub->diry;
	left_y = cub->dirx;
	if (mlx_is_key_down(cub->mlx, MLX_KEY_W))
		try_move(cub, cub->dirx * ms, cub->diry * ms);
	if (mlx_is_key_down(cub->mlx, MLX_KEY_S))
		try_move(cub, -cub->dirx * ms, -cub->diry * ms);
	if (mlx_is_key_down(cub->mlx, MLX_KEY_A))
		try_move(cub, left_x * ms, left_y * ms);
	if (mlx_is_key_down(cub->mlx, MLX_KEY_D))
		try_move(cub, -left_x * ms, -left_y * ms);
}

static void	update_rotate(t_cub *cub, double rs)
{
	if (mlx_is_key_down(cub->mlx, MLX_KEY_LEFT))
		rotate_player(cub, rs);
	if (mlx_is_key_down(cub->mlx, MLX_KEY_RIGHT))
		rotate_player(cub, -rs);
}

static void	on_loop(void *param)
{
	t_cub	*cub;
	double	dt;
	double	ms;
	double	rs;

	cub = (t_cub *)param;
	dt = cub->mlx->delta_time;
	ms = MOVE_S * dt;
	rs = ROT_S * dt;
	update_move(cub, ms);
	update_rotate(cub, rs);
	render_frame(cub);
}

int	main(int argc, char **argv)
{
	t_cub		cub;
	int			ret;
	const char	*map_path;

	map_path = NULL;
	if (argc > 1)
		map_path = argv[1];
	ret = init_game(&cub, map_path);
	if (ret != 0)
		error_exit();
	mlx_key_hook(cub.mlx, on_key, &cub);
	mlx_loop_hook(cub.mlx, on_loop, &cub);
	render_frame(&cub);
	mlx_loop(cub.mlx);
	tex_free(&cub.wall[TEX_NO]);
	tex_free(&cub.wall[TEX_SO]);
	tex_free(&cub.wall[TEX_WE]);
	tex_free(&cub.wall[TEX_EA]);
	free_map(&cub);
	if (cub.frame)
		mlx_delete_image(cub.mlx, cub.frame);
	mlx_terminate(cub.mlx);
	return (0);
}
