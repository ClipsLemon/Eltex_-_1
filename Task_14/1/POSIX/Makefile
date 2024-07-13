GCC = gcc



all: clean build

build:
	$(GCC) client.c -o client -lrt
	$(GCC) server.c -o server -lrt


format:
	clang-format -i --style=Google *.c

valgrind_test: clean $(OBJS) build
	valgrind --tool=memcheck --leak-check=full --track-origins=yes -s ./main_3

leaks_test: clean $(OBJS) build
	leaks -atExit -- ./main_3

git: clean
	git add -A
	git commit -m "authomatic commit for test"
	git push origin main

clean:
	rm -fr ./client ./server
