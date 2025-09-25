#ifndef CUB3D_H
#define CUB3D_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include "MLX42/include/MLX42/MLX42.h"\

#define WIDTH  1200
#define HEIGHT 1200
#define SPEED  200.0f

typedef struct s_cub {
    mlx_t*       mlx;
    mlx_image_t* player;
    int32_t      player_inst;
    float        px;
    float        py;
    char *map;
} t_cub;

typedef struct s_XY {
    int rotder;
    int rotspd;
} t_XY;

#endif