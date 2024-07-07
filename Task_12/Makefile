CC = gcc

SRC_DIR = ./3/core
OBJ_DIR = ./3/objects

# Получение имен подкаталогов в директории SRC_DIR
SUBDIRS := $(shell find $(SRC_DIR) -type d)

# Получение списка исходных файлов
SRCS := $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.c))

# Преобразование имен исходных файлов в имена объектных файлов
OBJS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))


all: clean build

build:  build_1 build_2 build_3

build_1:
	$(CC) main_1.c -o main_1

build_2:
	$(CC) ./2/server.c -o server
	$(CC) ./2/client.c -o client


# компиляци файлов папки src
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@

build_3: $(OBJS)
	$(CC) -c ./3/main.c -o main_3.o
	$(CC) $(OBJ_DIR)/$(SRC_DIR)/*.o ./main_3.o -o main_3

run:
	./main

format:
	clang-format -i --style=Google core/*.c *.c *.h

valgrind_test: clean $(OBJS) build
	valgrind --tool=memcheck --leak-check=full --track-origins=yes -s ./main_3

leaks_test: clean $(OBJS) build
	leaks -atExit -- ./main_3

git: clean
	git add -A
	git commit -m "authomatic commit for test"
	git push origin main

clean:
	rm -fr ./$(OBJ_DIR)/* ./main ./main.o
	rm -fr ./*.o ./main_1 ./main_3
	rm -fr ./client ./server ./pipe_fifo
