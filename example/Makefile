NAME = snake
CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -Wextra -Wno-unused-result -O3
SRC_DIR = .
INC_DIR = ../include
OUT_DIR = ./build

all: $(NAME)

$(NAME): $(SRC_DIR)/main.c $(SRC_DIR)/canvas.h $(INC_DIR)/snake.h
	mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS) -I $(INC_DIR) -o $(OUT_DIR)/$(NAME) $(SRC_DIR)/main.c

clean:
	rm -fr $(OUT_DIR)