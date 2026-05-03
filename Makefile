NAME := soundex
CC := gcc
CFLAGS := -Wall -Wextra -Werror -Iinclude
LDFLAGS := -lncurses
SRC_DIR := src
BIN_DIR := bin
INSTALL_DIR := $(HOME)/.local/bin
TARGET := $(BIN_DIR)/$(NAME)

SRC := $(wildcard $(SRC_DIR)/*.c)
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

.PHONY: all clean fclean re asan
