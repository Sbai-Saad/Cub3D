#include "Cub3D.h"

char map_at(t_cub* c, int x, int y)
{
    if (!c->map) return '1';
    if (x < 0 || y < 0 || x >= c->map_w || y >= c->map_h) return '1';
    return c->map[y][x];
}

static int clampi(int v, int lo, int hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

void render_frame(t_cub* c)
{
    int y = 0;
    while (y < HEIGHT) {
        unsigned int col;
        if (y < HEIGHT/2) col = c->ceil_color;
        else col = c->floor_color;
        int xclr = 0;
        while (xclr < WIDTH) {
            put_px(c->frame, xclr, y, col);
            ++xclr;
        }
        ++y;
    }

    int x = 0;
    while (x < WIDTH) {
        double cameraX = 2.0 * x / (double)WIDTH - 1.0;
        double rayDirX = c->dirX + c->planeX * cameraX;
        double rayDirY = c->dirY + c->planeY * cameraX;

        int mapX = (int)c->posX;
        int mapY = (int)c->posY;

        double deltaDistX;
        double deltaDistY;
        if (rayDirX == 0.0) deltaDistX = 1e30; else deltaDistX = fabs(1.0 / rayDirX);
        if (rayDirY == 0.0) deltaDistY = 1e30; else deltaDistY = fabs(1.0 / rayDirY);

        double sideDistX, sideDistY;
        int stepX, stepY;

        if (rayDirX < 0)
        {
            stepX = -1;
            sideDistX = (c->posX - mapX) * deltaDistX;
        }
        else             { stepX =  1; sideDistX = (mapX + 1.0 - c->posX) * deltaDistX; }
        if (rayDirY < 0) { stepY = -1; sideDistY = (c->posY - mapY) * deltaDistY; }
        else             { stepY =  1; sideDistY = (mapY + 1.0 - c->posY) * deltaDistY; }

        int hit = 0, side = 0;
        while (!hit) {
            if (sideDistX < sideDistY) { sideDistX += deltaDistX; mapX += stepX; side = 0; }
            else                       { sideDistY += deltaDistY; mapY += stepY; side = 1; }
            char cell = map_at(c, mapX, mapY);
            if (cell != '0') hit = 1;
        }

        double perpWallDist;
        if (side == 0) {
            double denom = rayDirX;
            if (denom == 0.0) denom = 1e-9;
            perpWallDist = (mapX - c->posX + (1 - stepX) / 2.0) / denom;
        } else {
            double denom = rayDirY;
            if (denom == 0.0) denom = 1e-9;
            perpWallDist = (mapY - c->posY + (1 - stepY) / 2.0) / denom;
        }
        if (perpWallDist < 1e-6) perpWallDist = 1e-6;

        int lineH  = (int)(HEIGHT / perpWallDist);
        int startY = clampi(-lineH / 2 + HEIGHT / 2, 0, HEIGHT - 1);
        int endY   = clampi( lineH / 2 + HEIGHT / 2, 0, HEIGHT - 1);

        int texid;
        if (side == 0) {
            if (rayDirX > 0) texid = TEX_WE;
            else texid = TEX_EA;
        } else {
            if (rayDirY > 0) texid = TEX_NO;
            else texid = TEX_SO;
        }

        double wallX;
        if (side == 0) wallX = c->posY + perpWallDist * rayDirY;
        else           wallX = c->posX + perpWallDist * rayDirX;
        wallX -= floor(wallX);

        t_tex* T = NULL;
        if (texid >= 0 && texid < TEX_MAX && c->has_tex[texid]) T = &c->wall[texid];

        if (!T || !T->pixels) {
            unsigned int wall;
            if (side == 0) wall = pack_rgba(200,200,200,255);
            else wall = pack_rgba(150,150,150,255);
            int yflat = startY;
            while (yflat <= endY) {
                put_px(c->frame, x, yflat, wall);
                ++yflat;
            }
        } else {
            int texW = (int)T->w, texH = (int)T->h;
            int texX = (int)(wallX * texW);
            double step = (double)texH / (double)lineH;
            double texPos = (startY - HEIGHT / 2.0 + lineH / 2.0) * step;

            int ytex = startY;
            while (ytex <= endY) {
                int texY = (int)texPos;
                texPos += step;
                uint32_t rgba = tex_sample_rgba(T, texX, texY);
                put_px(c->frame, x, ytex, rgba);
                ++ytex;
            }
        }
        ++x;
    }
}