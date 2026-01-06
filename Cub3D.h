#ifndef CUB3D_H
#define CUB3D_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include "MLX42/include/MLX42/MLX42.h"

#define WIDTH   800
#define HEIGHT  600
#define MOVE_S  3.0
#define ROT_S   1.8

typedef struct s_tex {
    mlx_texture_t* handle;
    uint32_t       w;
    uint32_t       h;
    uint8_t*       pixels;
} t_tex;

enum e_texid { TEX_NO = 0, TEX_SO, TEX_WE, TEX_EA, TEX_MAX };

typedef struct s_cub {
    mlx_t*        mlx;
    mlx_image_t*  frame;        
    char**        map;      
    int           map_w;
    int           map_h;
    char*         tex_path[TEX_MAX];
    double        posX, posY;
    double        dirX, dirY;
    double        planeX, planeY;
    unsigned int  ceil_color;
    unsigned int  floor_color;
    t_tex         wall[TEX_MAX];
    bool          has_tex[TEX_MAX];
} t_cub;

unsigned int pack_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void*        ft_memset(void *s, int c, size_t n);
uint32_t     tex_sample_rgba(const t_tex* t, int x, int y);
void         put_px(mlx_image_t* img, int x, int y, unsigned int col);
bool        tex_load_xpm42(t_tex* t, const char* path);
void        tex_free(t_tex* t);
void        render_frame(t_cub* c);
char        map_at(t_cub* c, int x, int y);
int         parse_map_file(t_cub* c, const char* path);
void        free_map(t_cub* c);
#endif