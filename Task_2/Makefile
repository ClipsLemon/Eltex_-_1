CFLAGS = -std=c11 -g -Wall -Wextra -Werror
CC = gcc

all: build_1 build_2 build_3 build_4

build_1:
	$(CC) $(CFLAGS) main_1.c -o main_1

build_2:
	$(CC) $(CFLAGS) main_2.c -o main_2

build_3:
	$(CC) $(CFLAGS) main_3.c -o main_3

build_4:
	$(CC) $(CFLAGS) main_4.c -o main_4

format:
	clang-format -i --style=Google *.c

clean:
	rm -fr ./main_1 ./main_2 ./main_3 ./main_4