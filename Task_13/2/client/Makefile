GCC = gcc -g 

SRC_DIR = src
OBJ_DIR = objects
INTERFACES_DIR = interface
FILES_FUNC = files_func
DIR_FUNC = dir_func
A := $(sudo ps -a)


LIBS = -lrt -pthread -lncurses
# Получение имен подкаталогов в директории SRC_DIR
SUBDIRS := $(shell find $(SRC_DIR) -type d)

# Получение списка исхsrcодных файлов
SRCS := $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.c))

# Преобразование имен исходных файлов в имена объектных файлов
OBJS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))



all: $(OBJS) build run 

# компиляци файлов папки src
$(OBJ_DIR)/%.o: %.c client.h ../color.h
	@mkdir -p $(dir $@)
	$(GCC) -c $< -o $@

build:
	$(CC) -c main.c -o main.o
	$(CC) $(OBJS) ./main.o -o client $(LIBS)

# build:
# 	$(GCC) ./$(OBJ_DIR)/*.o -o ./client


run:
	sudo ./client

format:
	clang-format -i --style=Google $(SRC_DIR)/*.c *.c *.h

valgrind_test: clean $(OBJS) build
	valgrind --tool=memcheck --leak-check=full --track-origins=yes -s ./client 

leaks_test: clean $(OBJS) build
	leaks -atExit -- ./client

clean:
	rm -fr $(OBJS) ./client ./*.o
	rm -fr ./client

gdb_client:
	$(A)