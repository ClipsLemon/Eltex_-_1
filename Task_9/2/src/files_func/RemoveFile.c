#include "../../main.h"

void RemoveFile(struct Directory *dir) {
  char filename[FILENAME_LEN];

  chdir(dir->path);
  // если мы можем получить доступ на запись в этот объект, то удаляем его
  if (access(dir->dir_list[dir->position]->d_name, W_OK) == 0) {
    // если его возможно удалить, то удаляем
    if (remove(dir->dir_list[dir->position]->d_name) != 0) {
      if (errno == 22) {
        ShowMessage("You cant remove this directory!", 3, 33,
                    getmaxx(stdscr) / 2 - 16, getmaxy(stdscr) / 2 - 1, 5);
      } else if (errno == 39) {
        ShowMessage("Directory not empty!", 3, 22, getmaxx(stdscr) / 2 - 11,
                    getmaxy(stdscr) / 2 - 1, 5);
      }
    }
  } else {
    ShowMessage("You have not enough rights!", 3, 29, getmaxx(stdscr) / 2 - 14,
                getmaxy(stdscr) / 2 - 1, 5);
  }
}
