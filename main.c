#include "header.h"

# define ESC_KEY 65307
# define W_KEY 119
# define S_KEY 115
# define A_KEY 97
# define D_KEY 100

# define SPEED 20.0f

typedef struct s_cub
{
    void    *mlx;
    void    *win;
    char    **map;
    void    *player;
    void    *no_texture;
    void    *we_texture;
    void    *ea_texture;
    void    *so_texture;
    int     floor_clr;
    int     celling_clr;
    float   px;
    float   py;
}t_cub;


void ft_mlx_init(t_cub *cub)
{
    cub->mlx = mlx_init();
    cub->win = mlx_new_window(cub->mlx, 800, 600, "Cub3D");
}

void init_imgs(t_cub *cub)
{
    int size;

    cub->player = mlx_xpm_file_to_image(cub->mlx, "xpm/gun.xpm", &size, &size);
}

void    ft_exit(t_cub *cub)
{
    mlx_destroy_image(cub->mlx, cub->player);
    mlx_destroy_display(cub->mlx);
    exit(0);
}

int buttons(int key, t_cub *cub)
{
    if (key == ESC_KEY)
        ft_exit(0);
    else if (key == D_KEY)
        cub->px += SPEED;
    else if (key == A_KEY)
      cub->px -= SPEED;
    else if (key == W_KEY)
      cub->py += SPEED;
    else if (key == S_KEY)
      cub->py -= SPEED;
}

void    init_cub(t_cub *cub, char *path)
{
    memset(cub, 0, sizeof(t_cub));
    ft_mlx_init(cub);
    // parse(cub, path);
    init_imgs(cub);
}

int main(int ac, char **av)
{
    t_cub   cub;


    init_cub(&cub, av[1]);

    mlx_put_image_to_window(cub.mlx, cub.win, cub.player, 0, 0);

    mlx_key_hook(cub.win, buttons, &cub);
    mlx_loop(cub.mlx);
}