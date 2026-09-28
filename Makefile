NAME   = codexion
SRC    = src/create_data.c src/parser.c src/threads.c main.c
OBJ    = $(SRC:.c=.o)
CFLAGS = -Wall -Wextra -Werror -g -I include
CC     = gcc

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -pthread -o $(NAME)

clean:
	rm -f $(OBJ)
	clear

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re