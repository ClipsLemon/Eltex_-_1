all: build_1 build_2 build_3

build_1:
	gcc main_1.c -o main_1

build_2:
	gcc main_2.c -o main_2

build_3:
	gcc main_3.c -o main_3

format:
	clang-format -i --style=Google *.c

clean:
	rm -fr ./main_1 ./main_2 ./main_3

