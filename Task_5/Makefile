CFLAGS = -std=c11 -g -fno-stack-protector -no-pie
CC = gcc

all: build_2

build_2:
	$(CC) $(CFLAGS) main_2.c -o main_2

format:
	clang-format -i --style=Google *.c

clean:
	rm -fr ./main_2