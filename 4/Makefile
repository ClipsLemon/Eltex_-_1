CFLAGS = -std=c11 -g -Wall -Wextra -Werror
CC = gcc

SRC_DIR = src
OBJ_DIR = objects

# Получение имен подкаталогов в директории SRC_DIR
SUBDIRS := $(shell find $(SRC_DIR) -type d)

# Получение списка исходных файлов
SRCS := $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.c))

# Преобразование имен исходных файлов в имена объектных файлов
OBJS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))



all: $(OBJS) build run

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

build:
	$(CC) $(CFLAGS) $(OBJ_DIR)/$(SRC_DIR)/*.o -o main

run:
	./main

format:
	clang-format -i --style=Google src/*.c

clean:
	rm -fr ./$(OBJ_DIR)/$(SRC_DIR)
	rm -fr ./main