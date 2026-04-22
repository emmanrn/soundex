NAME := soundex
CC := gcc
CFLAGS := -Wall -Wextra -Werror -Iinclude

SRC := src/main.c src/error.c src/file.c src/soundex.c src/dynarray.c src/fuzzy_search.c src/dice_co.c src/merge_sort.c
OBJ := $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) -o $@ $(OBJ)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

#address sanitizer
asan: CFLAGS += -g3 -fsanitize=address
asan: re

.PHONY: all clean fclean re asan