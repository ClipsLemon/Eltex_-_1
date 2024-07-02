

CC = gcc

SRC_DIR = src
OBJ_DIR = objects

# Получение имен подкаталогов в директории SRC_DIR
SUBDIRS := $(shell find $(SRC_DIR) -type d)

# Получение списка исходных файлов
SRCS := $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.c))

# Преобразование имен исходных файлов в имена объектных файлов
OBJS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))



all: clean $(OBJS) build run

# компиляци файлов папки src
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@

build:
	$(CC) -c main.c -o main.o
	$(CC) $(OBJ_DIR)/$(SRC_DIR)/*.o ./main.o -lncurses -o main

run:
	./main

format:
	clang-format -i --style=Google src/*.c *.c *.h

valgrind_test: clean $(OBJS) build
	valgrind --tool=memcheck --leak-check=full --track-origins=yes -s ./main 

leaks_test: clean $(OBJS) build
	leaks -atExit -- ./main

clean:
	rm -fr ./$(OBJ_DIR)/$(SRC_DIR) ./main.o
	rm -fr ./main