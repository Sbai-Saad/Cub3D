CC    = cc
CFLAGS = -Wall -Wextra -Werror -I.
NAME  = Cub3D
SRCS  = main.c\
		mainUtils.c\
		raycast.c\
		texture.c\
		parsing/parse.c\
		parsing/parse_utils.c\
		parsing/parse_io.c\
		parsing/parse_spawn.c\
		parsing/parse_color.c\
		parsing/parse_header.c\
		parsing/parse_validate.c\
		parsing/parse_free.c\
		parsing/parse_map_fill.c\
		util.c
OBJS  = $(SRCS:.c=.o)
MLX   = ./MLX42/build/libmlx42.a

GLFW_LIBS := $(shell pkg-config --static --libs glfw3 2>/dev/null)

LIBS  = $(MLX) $(GLFW_LIBS)


all: $(NAME)

$(NAME): $(OBJS) Cub3D.h
	$(CC) $(OBJS) $(LIBS) -lglfw -lm -o $(NAME)

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all
