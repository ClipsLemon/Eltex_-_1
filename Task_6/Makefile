CFLAGS = -std=c11 -g -Wall -Wextra -Werror
# CFLAGS = -std=c11 -g

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
	$(CC) $(CFLAGS) -c $< -o $@

build:
	$(CC) $(CFLAGS) -c main.c -o main.o
	$(CC) $(CFLAGS) $(OBJ_DIR)/$(SRC_DIR)/*.o ./main.o -o main

run:
	clear
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