#include "../../main.h"

void FindFile(struct Directory *dir) {
  char filename[FILENAME_LEN];
  int is_finded = 0;
  GetString(filename, "Enter filename:");

  for (int i = 0; i < dir->dir_len; i++) {
    if (strcmp(filename, dir->dir_list[i]->d_name) == 0) {
      dir->position = i;
      is_finded = 1;
      break;
    }
  }
  if (!is_finded) {
    PrintDir(&(window_list_ptr[LEFT_WINDOW]), home_path_ptr);
    PrintDir(&(window_list_ptr[RIGHT_WINDOW]), home_path_ptr);
    ShowMessage("No such file!", 3, 15, getmaxx(stdscr) / 2 - 7,
                getmaxy(stdscr) / 2 - 1, 5);
  }
}