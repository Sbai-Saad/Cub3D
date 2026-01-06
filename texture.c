#include "Cub3D.h"

bool tex_load_xpm42(t_tex* t, const char* path) {
    if (!t || !path) return false;
    xpm_t* xpm = mlx_load_xpm42(path);
    if (!xpm) return false;
    t->handle = (mlx_texture_t*)&xpm->texture;
    t->w = xpm->texture.width;
    t->h = xpm->texture.height;
    size_t sz = (size_t)t->w * t->h * 4;
    uint8_t* buf = malloc(sz);
    if (!buf) {
        mlx_delete_xpm42(xpm);
        t->handle = NULL; t->pixels = NULL; t->w = t->h = 0;
        return false;
    }
    size_t i = 0;
    while (i < sz) { buf[i] = xpm->texture.pixels[i]; ++i; }
    t->handle = NULL;
    t->pixels = buf;
    mlx_delete_xpm42(xpm);
    return true;
}

void tex_free(t_tex* t) {
    if (!t) return;
    if (!t->handle && t->pixels) {
        free(t->pixels);
    }
    if (t->handle) {
        mlx_delete_texture(t->handle);
    }
    t->handle = NULL;
    t->pixels = NULL;
    t->w = t->h = 0;
}
