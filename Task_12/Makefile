CC = gcc 

all: clean build

build: build_1 build_2

build_1:
	$(CC) main_1.c -o main_1

build_2:
	$(CC) ./2/server.c -o server
	$(CC) ./2/client.c -o client


# build_2:
# 	$(CC) -c main_2.c -o main_2.o
# 	$(CC) ./main_2.o -o main_2

# build_3:
# 	$(CC) -c main_3.c -o main_3.o
# 	$(CC) ./main_3.o -o main_3

format:
	clang-format -i --style=Google *.c


leaks_test: build
	leaks -atExit -- ./main_1

clean:
	rm -fr ./main_1.o ./main_1
	rm -fr ./client ./server ./pipe_fifo
