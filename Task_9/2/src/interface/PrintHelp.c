#include "../../main.h"

void PrintHelp() {
  WINDOW *help_win = newwin(8, 50, 10, 10);
  int choice = ' ';

  keypad(help_win, true);
  wbkgd(help_win, COLOR_PAIR(4));
  
  while (choice != 27) {
    mvwprintw(help_win, 1, 1, "\"Tab\" - switch between windows");
    mvwprintw(help_win, 2, 1, "\"Enter\" - open file or directory");
    mvwprintw(help_win, 3, 1, "\"Down arrow\" - go down the folder");
    mvwprintw(help_win, 4, 1, "\"Down up\" - go up the folder");
    mvwprintw(help_win, 5, 1, "\"q\" - get out of program");
    mvwprintw(help_win, 6, 1, "\"ESC\" - close message window");

    choice = wgetch(help_win);

    wrefresh(help_win);
  }
  delwin(help_win);
}