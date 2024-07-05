#include "../../main.h"

int is_program_installed(const char *program) {
  char command[256];
  snprintf(command, sizeof(command), "which %s > /dev/null 2>&1", program);
  int result = system(command);
  return result == 0;
}

void ClearString(char *string, int len) {
  for (int i = 0; i < len; i++) {
    string[i] = '\0';
  }
}

void OpenFile(struct Directory *dir, int *dir_changed, int which_window) {
  int status = 0;
  int child_pid = 0;
  int choice = ' ';
  int which_colored = 0;
  char command[256];

  // WINDOW *open_menu =
  //     subwin(dir->win, text_editor_count, 15, getmaxy(stdscr) / 2 + 10,
  //            getmaxx(stdscr) / 2 + 10);
  WINDOW *open_menu = newwin(text_editor_count, 15, dir->position + 4,
                             (getmaxx(stdscr) * which_window) / 2 + 4);

  wbkgd(open_menu, COLOR_PAIR(4));
  halfdelay(10);
  wclear(open_menu);
  keypad(open_menu, true);
  while (1) {
    if (choice == KEY_DOWN) {
      if (which_colored < text_editor_count - 1) {
        which_colored++;
      }
    } else if (choice == KEY_UP) {
      if (which_colored > 0) {
        which_colored--;
      }
    } else if (choice == 27) {
      break;
    } else if (choice == '\n') {
      chdir(dir->path);

      ClearString(command, 256);
      strcat(command, editor_list[which_colored]);
      strcat(command, " ");
      strcat(command, dir->dir_list[dir->position]->d_name);
      wrefresh(open_menu);
      system(command);

      curs_set(FALSE);
      keypad(dir->win, true);
      break;
    }
    for (int i = 0; i < text_editor_count; i++) {
      if (i == which_colored) {
        wattron(open_menu, COLOR_PAIR(2));
      }
      mvwprintw(open_menu, i, 1, "%s", editor_list[i]);
      if (i == which_colored) {
        wattroff(open_menu, COLOR_PAIR(2));
      }
    }
    wrefresh(open_menu);
    choice = wgetch(open_menu);
  }
  *dir_changed = 1;
  delwin(open_menu);
}