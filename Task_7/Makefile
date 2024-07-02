CFLAGS = -std=c11 -Wall -Wextra -Werror

CC = gcc

SRC_DIR = src
LIB_DIR = core
OBJ_DIR = objects

LIB_NAME = calculator.a

# Получение имен подкаталогов в директории SRC_DIR
SUBDIRS := $(shell find $(SRC_DIR) -type d)

# Получение списка исходных файлов
SRCS := $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.c))

# Преобразование имен исходных файлов в имена объектных файлов
OBJS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))



all: clean build run

# компиляци файлов папки src
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build: $(OBJS) build_library
	$(CC) $(CFLAGS) -c main.c -o main.o
	$(CC) $(CFLAGS)  $(OBJS) ./main.o -L. -l:calculator.a -o main 

build_library:
	ar rc $(LIB_NAME) $(OBJ_DIR)/$(SRC_DIR)/$(LIB_DIR)/*.o

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
	rm -fr $(LIB_NAME)
	rm -fr ./main