NAME := soundex
CC := gcc
CFLAGS := -Wall -Wextra -Werror -Iinclude
LDFLAGS := -lncurses
SRC_DIR := src
BIN_DIR := bin
INSTALL_DIR := $(HOME)/.local/bin
TARGET := $(BIN_DIR)/$(NAME)
UNAME := $(shell uname)

SRC := $(filter-out $(SRC_DIR)/linux_platform.c $(SRC_DIR)/win_platform.c, $(wildcard $(SRC_DIR)/*.c))

ifeq ($(UNAME), Linux)
    SRC += $(SRC_DIR)/linux_platform.c
#     LDFLAGS += -lncurses
else
    SRC += $(SRC_DIR)/win_platform.c
endif

OBJ := $(SRC:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)

all: $(BIN_DIR) $(TARGET)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)
# $@ ----> $(TARGET)
# $^ ----> $(OBJ)
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

install: all
	mkdir -p $(INSTALL_DIR)
	cp $(TARGET) $(INSTALL_DIR)/$(NAME)

test_match: src/match.c src/test_match.c
	$(CC) $(CFLAGS) -o bin/test_match src/match.c src/test_match.c
	./bin/test_match

uninstall: 
	rm -f $(INSTALL_DIR)/$(NAME)

clean:
	rm -rf $(BIN_DIR)

fclean: clean
	rm -f $(BIN_DIR)/$(NAME)

re: fclean all

#address sanitizer
asan: CFLAGS += -g3 -fsanitize=address
asan: re

.PHONY: all clean fclean re asan test_match
