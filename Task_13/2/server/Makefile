GCC = gcc -g

SRC_DIR = 
OBJ_DIR = objects
INTERFACES_DIR = interface
FILES_FUNC = files_func
DIR_FUNC = dir_func


# Получение имен подкаталогов в директории SRC_DIR
SUBDIRS := $(shell find $(SRC_DIR) -type d)

# Получение списка исхsrcодных файлов
SRCS := $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.c))

# Преобразование имен исходных файлов в имена объектных файлов
OBJS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))



all: $(OBJS) build run 

# компиляци файлов папки src
$(OBJ_DIR)/%.o: %.c server.h color.h
	@mkdir -p $(dir $@)
	$(GCC) -c $< -o $@

# build:
# $(CC) -c main.c -o main.o
# $(CC) $(OBJ_DIR)/$(SRC_DIR)/*.o $(OBJ_DIR)/$(SRC_DIR)/$(INTERFACES_DIR)/*.o $(OBJ_DIR)/$(SRC_DIR)/$(FILES_FUNC)/*.o $(OBJ_DIR)/$(SRC_DIR)/$(DIR_FUNC)/*.o ./main.o -lncurses -o server

build:
	$(GCC) ./$(OBJ_DIR)/*.o -o ./server


run:
	sudo ./server

format:
	clang-format -i --style=Google $(SRC_DIR)/*.c *.c *.h

valgrind_test: clean $(OBJS) build
	valgrind --tool=memcheck --leak-check=full --track-origins=yes -s ./server 

leaks_test: clean $(OBJS) build
	leaks -atExit -- ./server

clean:
	rm -fr ./$(OBJ_DIR)/* ./server ./*.o
	rm -fr ./server