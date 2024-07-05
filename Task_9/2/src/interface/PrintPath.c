#include "../../main.h"

void PrintPath(struct Directory *win, char *home_path) {
  mvwprintw(win->win, 1, 1, "CURRENT PATH: %-*s", getmaxx(win->win) - 15,
            win->path);
  mvwprintw(win->win, 2, 1, "HOME DIR: %-*s", getmaxx(win->win) - 15,
            home_path);
}
