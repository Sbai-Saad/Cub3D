MLX42_DIR ?= ./MLX42

CC    = cc
NAME  = Cub3D
SRCS  = main.c
OBJS  = $(SRCS:.c=.o)
LIBS  = $(MLX42_DIR)/build/libmlx42.a $(shell pkg-config --libs glfw3 2>/dev/null)

all: $(NAME)

$(NAME): $(OBJS) Cub3D.h
	$(CC) $(OBJS) $(LIBS) -o $(NAME)

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

test: 
	cc test.c $(LIBS) -o test
rmt:
	rm test