#include "../../main.h"

void FreeDirList(struct Directory *win) {
  for (int i = 0; i < win->dir_len; i++) {
    free(win->dir_list[i]);
  }
  free(win->dir_list);
}