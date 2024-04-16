NAME=snake-server
CC=gcc
CFLAGS=-Wall -O2 
SRC=src/$(NAME).c
OUT=bin

all: $(NAME)

$(NAME):
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)/$(NAME)

clean:
	rm $(OUT)/$(NAME) 

.PHONY: all $(NAME) clean
