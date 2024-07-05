#ifndef MAIN_H
#define MAIN_H

#include <dirent.h>
#include <errno.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define PATH_LEN 300
#define HOME_PATH_LEN 50
#define COLOR_BACKGROUND 3
#define FILENAME_LEN 100

#define MAX_MODIFY_DATE_WIDTH 21
#define COLOR_DIR 2
#define COLOR_FILE 1

// ширина поля "Размер" в строке
#define SIZE_WIDTH 10

#define LEFT_WINDOW 0
#define RIGHT_WINDOW 1

// ширина поля "Последняя модификация" в строке
#define MOD_TIME_WIDTH 15

struct Directory {
  struct dirent **dir_list;
  char path[PATH_LEN];
  int position;
  int dir_len;
  WINDOW *win;
};

extern struct Directory *window_list_ptr;
extern char *home_path_ptr;
extern const char *text_editors[];
extern char **editor_list;

extern int text_editor_count;

/**
 * @brief Функция инициализации окна.
 *
 * @param win - указатель на окно
 * @param row - столбцы экрана (не окна, а экрана)
 * @param col - размеры экрана (не окна, а экрана)
 * @param y - начальная позиция окна
 * @param x - начальная позиция окна
 */
void InitWin(struct Directory *win, int row, int col, int y, int x);

void PrintPath(struct Directory *win, char *home_path);

void FreeDirList(struct Directory *win);

void ChangeDir(struct Directory *win, int *dir_changed_ptr);

void InitWin(struct Directory *win, int row, int col, int y, int x);

/**
 * @brief Функция выводит директорию в определенное окно
 * Так же функция не дает выйти за пределы корневой директории
 * @param win - указатель на окно
 * @param home_path - строка с домашним адресом
 */
void PrintDir(struct Directory *win, char *home_path);

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

void PrintManager(WINDOW *footer, struct Directory *window_list,
                  char *home_path);

void GetInput(struct Directory *dir, int *ch, int *dir_changed,
              int *current_window);

void PrintFooter(WINDOW *footer);

void CreateFile(char *path);

void GetString(char *string, char *title);

void CreateDir(char *path);

void RemoveFile(struct Directory *dir);

void ShowMessage(char *message, int row, int col, int x, int y, int color_pair);

void Create(char *path);

void PrintHelp();

void FindFile(struct Directory *dir);

void OpenFile(struct Directory *dir, int *dir_changed, int which_window);

int is_program_installed(const char *program);

void ClearString(char *string, int len);

#endif // MAIN_H