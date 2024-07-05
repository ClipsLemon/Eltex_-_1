CC = gcc -g

SRC_DIR = src
OBJ_DIR = objects
INTERFACES_DIR = interface
FILES_FUNC = files_func
DIR_FUNC = dir_func


# Получение имен подкаталогов в директории SRC_DIR
SUBDIRS := $(shell find $(SRC_DIR) -type d)

# Получение списка исходных файлов
SRCS := $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.c))

# Преобразование имен исходных файлов в имена объектных файлов
OBJS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))



all: $(OBJS) build run

# компиляци файлов папки src
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@

build:
	$(CC) -c main.c -o main.o
	$(CC) $(OBJ_DIR)/$(SRC_DIR)/*.o $(OBJ_DIR)/$(SRC_DIR)/$(INTERFACES_DIR)/*.o $(OBJ_DIR)/$(SRC_DIR)/$(FILES_FUNC)/*.o $(OBJ_DIR)/$(SRC_DIR)/$(DIR_FUNC)/*.o ./main.o -lncurses -o manager

run:
	./manager

format:
	clang-format -i --style=Google src/*.c *.c *.h

valgrind_test: clean $(OBJS) build
	valgrind --tool=memcheck --leak-check=full --track-origins=yes -s ./manager 

leaks_test: clean $(OBJS) build
	leaks -atExit -- ./manager

clean:
	rm -fr ./$(OBJ_DIR)/$(SRC_DIR) ./manager ./main.o
	rm -fr ./manager