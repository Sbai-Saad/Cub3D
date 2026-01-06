CC    = cc
NAME  = Cub3D
SRCS  = main.c\
		raycast.c\
		texture.c\
		parse.c\
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

test: 
	cc test.c -lm $(LIBS) -o test
rmt:
	rm test