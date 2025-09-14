flags =
CC = cc
NAME = Cub3D
SRCS = main.c
OBJS = $(SRCS:.c=.o)
rm = rm -rf
MLXFLG = -Lminilibx-linux -lmlx -lXext -lX11 -lm

all: $(NAME)

$(NAME): $(OBJS) header.h
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(MLXFLG)

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all