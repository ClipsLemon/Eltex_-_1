CC = gcc -Wall -Wextra -Werror

all: clean build

build: build_1 build_2 build_3

build_1:
	$(CC) -c main_1.c -o main_1.o
	$(CC) ./main_1.o -o main_1

build_2:
	$(CC) -c main_2.c -o main_2.o
	$(CC) ./main_2.o -o main_2

build_3:
	$(CC) -c ./3/FindProgram.c -o FindProgram.o
	$(CC) -c ./3/LineParses.c -o LineParses.o
	$(CC) -c ./3/RemoveArgument.c -o RemoveArgument.o
	$(CC) -c ./3/String.c -o String.o

	$(CC) -c main_3.c -o main_3.o
	$(CC) FindProgram.o LineParses.o String.o RemoveArgument.o ./main_3.o -o main_3

format:
	clang-format -i --style=Google *.c *.h
	clang-format -i --style=Google 3/*.c


valgrind_test: clean build_3
	valgrind --tool=memcheck --leak-check=full --track-origins=yes ./main_3

clean:
	rm -fr ./main_1 ./main_2 ./main_3
	rm -fr ./*.o

