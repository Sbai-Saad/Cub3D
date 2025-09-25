/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasbai <sasbai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 07:39:49 by sasbai            #+#    #+#             */
/*   Updated: 2025/09/24 07:39:56 by sasbai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

static unsigned int rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return ((unsigned int)r << 24) | ((unsigned int)g << 16) | ((unsigned int)b << 8) | (unsigned int)a;
}

static void draw_placeholder(mlx_image_t* img)
{
    for (unsigned int y = 0; y < img->height; ++y) {
        for (unsigned int x = 0; x < img->width; ++x) {
            bool on = ((x / 16) + (y / 16)) & 1;
            unsigned int color;
            if (on) {
                color = rgba(200, 60, 60, 255);
            } else {
                color = rgba(60, 60, 200, 255);
            }
            mlx_put_pixel(img, x, y, color);
        }
    }
}

static mlx_image_t* load_png_or_xpm42(mlx_t* mlx, const char* png_path, const char* xpm42_path)
{
    mlx_texture_t* tex = mlx_load_png(png_path);
    if (tex)
    {
        mlx_image_t* img = mlx_texture_to_image(mlx, tex);
        mlx_delete_texture(tex);
        return img;
    }
    xpm_t* xpm = mlx_load_xpm42(xpm42_path);
    if (xpm)
    {
        mlx_image_t* img = mlx_texture_to_image(mlx, &xpm->texture);
        mlx_delete_xpm42(xpm);
        return img;
    }
    return NULL;
}

static void ft_mlx_init(t_cub* cub)
{
    cub->mlx = mlx_init(WIDTH, HEIGHT, "Cub3D", true);
    if (!cub->mlx)
    {
        fprintf(stderr, "mlx_init failed: %s\n", mlx_strerror(mlx_errno));
        exit(EXIT_FAILURE);
    }
}

static void init_imgs(t_cub* cub)
{
    cub->player = load_png_or_xpm42(cub->mlx, "xpm/hh.png", "xpm/hh.xpm42");
    if (!cub->player)
    {
        fprintf(stderr, "khas tswira %s.\n",mlx_strerror(mlx_errno));
        cub->player = mlx_new_image(cub->mlx, 128, 128);
        if (!cub->player)
        {
            fprintf(stderr, "tswira khsrat %s\n", mlx_strerror(mlx_errno));
            exit(EXIT_FAILURE);
        }
        draw_placeholder(cub->player);
    }

    cub->px = (WIDTH - (float)cub->player->width) / 2.0f;
    cub->py = (HEIGHT - (float)cub->player->height) / 2.0f;

    cub->player_inst = mlx_image_to_window(cub->mlx, cub->player, (int32_t)cub->px, (int32_t)cub->py);
    if (cub->player_inst < 0)
    {
        fprintf(stderr, "mlx_image_to_window failed: %s\n", mlx_strerror(mlx_errno));
        exit(EXIT_FAILURE);
    }
}

static void ft_exit(t_cub* cub)
{
    if (cub->player)
    {
        mlx_delete_image(cub->mlx, cub->player);
    }
    if (cub->mlx)
    {
        mlx_terminate(cub->mlx);
    }
    exit(0);
}

static void on_key(mlx_key_data_t kd, void* param)
{
    t_cub* cub = (t_cub*)param;
    if (kd.key == MLX_KEY_ESCAPE && kd.action == MLX_PRESS)
    {
        mlx_close_window(cub->mlx);
    }
}

static void on_loop(void* param)
{
    t_cub* cub = (t_cub*)param;
    float dt = (float)cub->mlx->delta_time;
    float dx = 0.0f, dy = 0.0f;

    if (mlx_is_key_down(cub->mlx, MLX_KEY_W) || mlx_is_key_down(cub->mlx, MLX_KEY_UP) )
        dy -= SPEED * dt;
    if (mlx_is_key_down(cub->mlx, MLX_KEY_S) || mlx_is_key_down(cub->mlx, MLX_KEY_DOWN) )
        dy += SPEED * dt;
    if (mlx_is_key_down(cub->mlx, MLX_KEY_A) || mlx_is_key_down(cub->mlx, MLX_KEY_LEFT) )
        dx -= SPEED * dt;
    if (mlx_is_key_down(cub->mlx, MLX_KEY_D) || mlx_is_key_down(cub->mlx, MLX_KEY_RIGHT) )
        dx += SPEED * dt;

    cub->px += dx;
    cub->py += dy;

    if (cub->px < 0) cub->px = 0;
    if (cub->py < 0) cub->py = 0;
    if (cub->px > WIDTH - (float)cub->player->width)
        cub->px = WIDTH - (float)cub->player->width;
    if (cub->py > HEIGHT - (float)cub->player->height)
        cub->py = HEIGHT - (float)cub->player->height;

    cub->player->instances[cub->player_inst].x = (int32_t)cub->px;
    cub->player->instances[cub->player_inst].y = (int32_t)cub->py;
}

void player_movemet(int tder)
{
    t_XY rot;
    tder = rot.rotder * rot.rotspd;
}

int main(int ac, char** av)
{
    (void)av;
    // if (ac != 2) {
    //     fprintf(stderr, "Usage: ./Cub3D <map.cub>\n");
    //     return 1;
    // }

    t_cub cub = {0};
    ft_mlx_init(&cub);
    init_imgs(&cub);
    mlx_key_hook(cub.mlx, on_key, &cub);
    mlx_loop_hook(cub.mlx, on_loop, &cub);
    mlx_loop(cub.mlx);
    ft_exit(&cub);
    return 0;
}