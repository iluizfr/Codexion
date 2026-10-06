NAME   = codexion
SRC    = src/create_data.c src/parser.c src/threads.c main.c
OBJ    = $(SRC:.c=.o)
CFLAGS = -Wall -Wextra -Werror -g -I include
CC     = gcc
DEFAULT_VALUE_FOR_TEST = 4 1000 100 100 100 3 100 "fifo"

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -pthread -o $(NAME)

run:
	./$(NAME) $(DEFAULT_VALUE_FOR_TEST)

clean:
	rm -f $(OBJ)
	clear

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re