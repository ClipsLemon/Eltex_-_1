#include "../../main.h"

#define FILE 1
#define DIR 2

void Create(char *path) {
  int stdscr_row = getmaxy(stdscr);
  int stdscr_col = getmaxx(stdscr);
  int window_row = 5;
  int window_col = 20;
  int choice = ' ';
  int type = 0;

  WINDOW *choice_window =
      newwin(window_row, window_col, stdscr_row / 2 - window_row / 2,
             stdscr_col / 2 - window_col / 2);

  keypad(choice_window, true);
  wbkgd(choice_window, COLOR_PAIR(4));

  mvwprintw(choice_window, 3, 1, "FILE");
  mvwprintw(choice_window, 3, 10, "DIRECTORY");
  // wrefresh(choice_window);

  while (1) {
    mvwprintw(choice_window, 1, 1, "File or Directory?");

    choice = wgetch(choice_window);
    if (choice == KEY_LEFT) {
      type = FILE;
      wclear(choice_window);
      wattron(choice_window, COLOR_PAIR(2));
      mvwprintw(choice_window, 3, 1, "FILE");
      wattroff(choice_window, COLOR_PAIR(2));
      mvwprintw(choice_window, 3, 10, "DIRECTORY");
    } else if (choice == KEY_RIGHT) {
      type = DIR;
      wclear(choice_window);
      mvwprintw(choice_window, 3, 1, "FILE");
      wattron(choice_window, COLOR_PAIR(2));
      mvwprintw(choice_window, 3, 10, "DIRECTORY");
      wattroff(choice_window, COLOR_PAIR(2));
    } else if (choice == 27) {
      delwin(choice_window);
      break;
    } else if (choice == '\n') {
      if (type == FILE) {
        delwin(choice_window);
        PrintDir(&(window_list_ptr[LEFT_WINDOW]), home_path_ptr);
        PrintDir(&(window_list_ptr[RIGHT_WINDOW]), home_path_ptr);
        CreateFile(path);
        break;
      } else if (type == DIR) {
        delwin(choice_window);
        PrintDir(&(window_list_ptr[LEFT_WINDOW]), home_path_ptr);
        PrintDir(&(window_list_ptr[RIGHT_WINDOW]), home_path_ptr);
        CreateDir(path);
        break;
      }
    }
    wrefresh(choice_window);
  }
}