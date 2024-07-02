CFLAGS = -std=c11 -Wall -Wextra -Werror
CC = gcc

all: clean build

build:
	$(CC) $(CFLAGS) main.c -o main

format:
	clang-format -i --style=Google *.c

clean:
	rm -fr ./main
	rm -fr output.txt