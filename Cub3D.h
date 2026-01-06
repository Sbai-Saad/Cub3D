#ifndef CUB3D_H
# define CUB3D_H

# include <stddef.h>
# include <stdint.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <math.h>
# include "MLX42/include/MLX42/MLX42.h"

# define WIDTH 800
# define HEIGHT 600

# define MOVE_S 3.0
# define ROT_S 1.8

typedef struct s_tex
{
	mlx_texture_t	*handle;
	uint32_t		w;
	uint32_t		h;
	uint8_t			*pixels;
}	t_tex;

enum e_texid
{
	TEX_NO = 0,
	TEX_SO,
	TEX_WE,
	TEX_EA,
	TEX_MAX
};

typedef struct s_vec
{
	double	x;
	double	y;
}	t_vec;

struct s_ray
{
	double	raydirx;
	double	raydiry;
	int	mapx;
	int	mapy;
	double	deltadistx;
	double	deltadisty;
	double	sidedistx;
	double	sidedisty;
	int	stepx;
	int	stepy;
	int	side;
	double	perpwalldist;
	int	lineh;
	int	starty;
	int	endy;
	double	wallx;
	int	texid;
};

typedef struct s_cub
{
	mlx_t			*mlx;
	mlx_image_t		*frame;

	char			**map;
	int				map_w;
	int				map_h;

	char			*tex_path[TEX_MAX];
	double			posx;
	double			posy;
	double			dirx;
	double			diry;
	double			planex;
	double			planey;

	unsigned int	ceil_color;
	unsigned int	floor_color;

	t_tex			wall[TEX_MAX];
	bool			has_tex[TEX_MAX];
}	t_cub;
void			error_exit(void);
void			try_move(t_cub *cub, double dx, double dy);
void			rotate_player(t_cub *cub, double angle);
int				load_textures(t_cub *cub);
int				init_game(t_cub *cub, const char *map_path);
size_t			s_len(const char *s);
int				count_lines(char *file, ssize_t len);
char			*copy_line(const char *s);
void			rstrip(char *s);
void			free_partial(char **rows, int count);
int				read_all(const char *path, char **out_buf, ssize_t *out_len);
int				ensure_spawn(t_cub *c, char dir, int mx, int my, int *has_spawn);
void			set_plane_from_dir(t_cub *c);
int				is_map_char(char ch);
int				parse_rgb(const char *s, unsigned int *out_rgba);
int				parse_header_line(t_cub *c, const char *line, int *tex_seen,
					int *floor_set, int *ceil_set);
int				validate_closed(t_cub *c);
int				line_is_map(const char *s);
int				alloc_map(t_cub *c, int map_h, int map_w);
int				fill_map_rows(t_cub *c, char **rows, int start, int map_h);
unsigned int	pack_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void			*ft_memset(void *s, int c, size_t n);
uint32_t		tex_sample_rgba(const t_tex *t, int x, int y);
void			put_px(mlx_image_t *img, int x, int y, unsigned int col);

bool			tex_load_xpm42(t_tex *t, const char *path);
void			tex_free(t_tex *t);

void			render_frame(t_cub *c);

char			map_at(t_cub *c, int x, int y);
int				parse_map_file(t_cub *c, const char *path);
void			free_map(t_cub *c);

#endif