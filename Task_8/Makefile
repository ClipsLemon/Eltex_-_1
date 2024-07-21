CFLAGS = -std=c11 -Wall -Wextra -Werror -fPIC
LDFLAGS = -shared

CC = gcc

SRC_DIR = src
LIB_DIR = core
OBJ_DIR = objects

LIB_NAME = libcalculator.so

# Получение имен подкаталогов в директории SRC_DIR
SUBDIRS := $(shell find $(SRC_DIR) -type d)

# Получение списка исходных файлов
SRCS := $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.c))

# Преобразование имен исходных файлов в имена объектных файлов
OBJS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))

all: clean build run

# компиляция файлов папки src
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build: $(OBJS) build_library
	$(CC) $(CFLAGS) -c main.c -o main.o
	$(CC) $(CFLAGS) main.o -L. -lcalculator -o main 

build_library: $(OBJS)
	$(CC) $(LDFLAGS) -o $(LIB_NAME) $(OBJS)

run: build
	LD_LIBRARY_PATH=. ./main

format:
	clang-format -i --style=Google src/*.c *.c *.h

valgrind_test: clean build
	valgrind --tool=memcheck --leak-check=full --track-origins=yes -s ./main 

leaks_test: clean build
	leaks -atExit -- ./main

clean:
	rm -fr $(OBJ_DIR) main.o main $(LIB_NAME)

