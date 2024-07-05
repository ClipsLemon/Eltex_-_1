#include "../main.h"

void GetString(char *string, char *title) {
  int row = getmaxy(stdscr);
  int col = getmaxx(stdscr);

  echo();
  WINDOW *user_input_window = newwin(4, FILENAME_LEN + 2, 10, 10);

  wbkgd(user_input_window, COLOR_PAIR(4));
  mvwprintw(user_input_window, 1, 1, title);
  mvwgetnstr(user_input_window, 2, 1, string, FILENAME_LEN);
  wrefresh(user_input_window);
  
  noecho();
  delwin(user_input_window);
}