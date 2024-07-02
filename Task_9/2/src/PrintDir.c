#include "../main.h"


void PrintDir(struct MyWin *win, char *home_path) {
  // количество строк в окне
  int row = getmaxy(win->win);
  // переменная даны
  char date[MAX_MODIFY_DATE_WIDTH];
  // стартовое смещение для вывода строк директории
  int start = 0;
  // статистика объекта строки директории
  struct stat object_stat;

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
    if (i == win->position && win->dir_list[i]->d_type == 4) {
      wattron(win->win, COLOR_PAIR(2));
    } else if (i == win->position) {
      wattron(win->win, COLOR_PAIR(1));
    }

    stat(win->dir_list[i]->d_name, &object_stat);

    strftime(date, MAX_MODIFY_DATE_WIDTH, "%Y %b %d %H:%M:%S",
             localtime(&(object_stat.st_mtime)));

    // непонятно почему надо уменьшать на 17
    mvwprintw(win->win, i + 3 - start, 2, "%-*s\t%*ldKB\t%s\t",
              (getmaxx(win->win) - MAX_MODIFY_DATE_WIDTH - SIZE_WIDTH - 17),
              win->dir_list[i]->d_name, SIZE_WIDTH, object_stat.st_size / 1024,
              date);

    if (i == win->position && win->dir_list[i]->d_type == 4) {
      wattroff(win->win, COLOR_PAIR(2));
    } else if (i == win->position) {
      wattroff(win->win, COLOR_PAIR(1));
    }
  }
  wrefresh(win->win); // обновить экран
}