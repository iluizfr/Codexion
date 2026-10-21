NAME   = codexion
SRC    = src/init.c src/parser.c src/run.c src/monitor_routine.c src/utils.c main.c \
		 src/coder_routine.c src/actions.c src/manager_routine.c src/time.c
OBJ    = $(SRC:.c=.o)
CFLAGS = -Wall -Wextra -Werror -g -I include
CC     = gcc
DEFAULT_VALUE_FOR_TEST = 2 700 200 200 200 4 200 "fifo"

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