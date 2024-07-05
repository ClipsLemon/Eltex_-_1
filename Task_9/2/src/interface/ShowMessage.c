#include "../../main.h"

void ShowMessage(char *message, int row, int col, int x, int y,
                 int color_pair) {
  WINDOW *message_window = newwin(row, col, y, x);
  halfdelay(40);
  wbkgd(message_window, COLOR_PAIR(5));
  box(message_window, 0, 0);
  mvwprintw(message_window, row / 2, col / 2 - strlen(message) / 2, message);
  wrefresh(message_window);
  wgetch(message_window);
  cbreak();
  delwin(message_window);
}