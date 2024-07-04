#ifndef MAIN_H
#define MAIN_H

#include <dirent.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define PATH_LEN 300
#define HOME_PATH_LEN 50
// #define MAX_STRING_LEN 200
#define COLOR_BACKGROUND 3

#define MAX_MODIFY_DATE_WIDTH 21
#define COLOR_DIR 2
#define COLOR_FILE 1

// ширина поля "Размер" в строке
#define SIZE_WIDTH 10

#define LEFT_WINDOW 0
#define RIGHT_WINDOW 1

// ширина поля "Последняя модификация" в строке
#define MOD_TIME_WIDTH 15

struct MyWin {
  struct dirent **dir_list;
  char path[PATH_LEN];
  int position;
  int dir_len;
  WINDOW *win;
};

/**
 * @brief Функция инициализации окна.
 *
 * @param win - указатель на окно
 * @param col - размеры экрана (не окна, а экрана)
 * @param row - столбцы экрана (не окна, а экрана)
 * @param y - начальная позиция окна
 * @param x - начальная позиция окна
 */
void InitWin(struct MyWin *win, int row, int col, int y, int x);

void PrintPath(struct MyWin *win, char *home_path);

void FreeDirList(struct MyWin *win);

void ChangeDir(struct MyWin *win, int *dir_changed_ptr);

void InitWin(struct MyWin *win, int row, int col, int y, int x);

/**
 * @brief Функция выводит директорию в определенное окно
 * Так же функция не дает выйти за пределы корневой директории
 * @param win - указатель на окно
 * @param home_path - строка с домашним адресом
 */
void PrintDir(struct MyWin *win, char *home_path);

/**
 * @brief Функция в текущем пути удаляет последний каталог и возвращает
 * измененный путь Сначала функция находит последний слэш, затем идет от него и
 * до конца строки (ближайшего \0) проставляет \0
 */
void PathDeleteDir(char *path);

/**
 * @brief Функция добавляет в путь папку в которую перешел пользователь
 *
 * @param path - текущий путь
 * @param added_dir - папка, в которую переходит пользователь
 */
void PathAddDir(char *path, char *added_dir);
#endif // MAIN_H