#include "../../main.h"

void PrintDir(struct Directory *win, char *home_path) {
  // количество строк в окне
  int row = getmaxy(win->win);
  // переменная даны
  char date[MAX_MODIFY_DATE_WIDTH];
  // стартовое смещение для вывода строк директории
  int start = 0;
  // статистика объекта строки директории
  // struct stat object_stat;

  // -4 потому что 2 строки уходит на обводку и 2 строки на путь
  if (win->position >= row - 4) {
    start = 5 - row + win->position;
    wclear(win->win);
    mvwprintw(win->win, 1, 1, "CURRENT PATH: %-*s", getmaxx(win->win) - 15,
              win->path);
    mvwprintw(win->win, 2, 1, "HOME DIR: %-*s", getmaxx(win->win) - 15,
              home_path);
  }

  box(win->win, 0, 0);
  box(win->win, 0, 0);

  for (int i = start, l = 0; i < win->dir_len && l < row - 4; i++, l++) {
    if (i == win->position &&
        (win->dir_list[i]->d_type == 4 || win->dir_list[i]->d_type == 10)) {
      wattron(win->win, COLOR_PAIR(2));
    } else if (i == win->position) {
      wattron(win->win, COLOR_PAIR(1));
    }

    mvwprintw(win->win, i + 3 - start, 2, "%-*s", (getmaxx(win->win) - 10),
              win->dir_list[i]->d_name);

    if (i == win->position &&
        (win->dir_list[i]->d_type == 4 || win->dir_list[i]->d_type == 10)) {
      wattroff(win->win, COLOR_PAIR(2));
    } else if (i == win->position) {
      wattroff(win->win, COLOR_PAIR(1));
    }
  }
  wrefresh(win->win); // обновить экран
}