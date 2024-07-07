CC = gcc

SRC_DIR = core
OBJ_DIR = objects
# INTERFACES_DIR = interface
# FILES_FUNC = files_func
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

build: main.c
	$(CC) -c main.c -o main.o
	$(CC) $(OBJ_DIR)/$(SRC_DIR)/*.o ./main.o -o main

run:
	./main

format:
	clang-format -i --style=Google core/*.c *.c *.h

valgrind_test: clean $(OBJS) build
	valgrind --tool=memcheck --leak-check=full --track-origins=yes -s ./main 

leaks_test: clean $(OBJS) build
	leaks -atExit -- ./main

git: clean
	git add -A
	git commit -m "authomatic commit for test"
	git push origin main

clean:
	rm -fr ./$(OBJ_DIR)/* ./main ./main.o
	rm -fr ./main