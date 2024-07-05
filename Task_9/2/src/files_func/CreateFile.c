#include "../../main.h"

void CreateFile(char *path) {
  char filename[FILENAME_LEN];

  chdir(path);
  // если не пустое имя
  GetString(filename, "Enter filename: ");
  if (strlen(filename)) {
    // если файла не существует
    if (access(filename, R_OK) != 0) {
      FILE *f = fopen(filename, "w");
      fclose(f);
    } else {
      PrintDir(&(window_list_ptr[LEFT_WINDOW]), home_path_ptr);
      PrintDir(&(window_list_ptr[RIGHT_WINDOW]), home_path_ptr);
      ShowMessage("File with such name exists!", 3, 29,
                  getmaxx(stdscr) / 2 - 14, getmaxy(stdscr) / 2 - 1, 5);
    }
  }
}