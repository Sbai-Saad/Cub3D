CC    = cc
CFLAGS = -Wall -Wextra -Werror -I.
NAME  = Cub3D
SRCS  = main.c\
		util.c\
		texture.c\
		mainUtils.c\
		mainUtils2.c\
		raycast/raycast.c\
		raycast/raycastUtils.c\
		raycast/raycastUtils2.c\
		parsing/parse.c\
		parsing/parse_io.c\
		parsing/parse_utils.c\
		parsing/parse_spawn.c\
		parsing/parse_color.c\
		parsing/parse_validate.c\
		parsing/parse_header.c\
		parsing/parse_free.c\
		parsing/parse_map_fill.c
OBJS  = $(SRCS:.c=.o)
MLX42   = ./MLX42/build/libmlx42.a

GLFW_LIBS := $(shell pkg-config --static --libs glfw3 2>/dev/null)
ifeq ($(strip $(GLFW_LIBS)),)
GLFW_LIBS := -lglfw -ldl -lm -lpthread -lGL -lX11
endif

LIBS  = $(MLX42) $(GLFW_LIBS) -lm


all: MLX $(NAME)


%.o: %.c Cub3D.h MLX42/include/MLX42/MLX42.h
	$(CC) -c $< -o $@ $(CFLAGS)

$(NAME): $(OBJS) 
	$(CC) $(OBJS) $(LIBS) -o $(NAME)

MLX:
	cd MLX42 && cmake -B build && cmake --build build

clean:
	@rm -f $(OBJS)

mlx_clean:
	@rm -rf MLX42/build 

fclean: clean mlx_clean
	@rm -f $(NAME) && echo "files are removed :P"

re: fclean all
