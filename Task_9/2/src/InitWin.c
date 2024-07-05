#include "../main.h"

void InitWin(struct Directory *win, int row, int col, int y, int x) {
  win->win = newwin(row, col, y, x);
  win->position = 0;
  getcwd(win->path, PATH_LEN);
  win->dir_len = scandir(win->path, &(win->dir_list), 0, alphasort);
  keypad(win->win, TRUE);
}