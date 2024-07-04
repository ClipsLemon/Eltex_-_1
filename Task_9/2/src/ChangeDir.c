#include "../main.h"

void ChangeDir(struct MyWin *win, int *dir_changed_ptr) {
  struct dirent **tmp_dir_list;
  int tmp_len = 0;
  char path_copy[PATH_LEN];

  if (win->dir_list[win->position]->d_type == 4) {
    if (strcmp(win->dir_list[win->position]->d_name, ".") &&
        strcmp(win->dir_list[win->position]->d_name, "..")) {
      strcpy(path_copy, win->path);
      strcat(path_copy, "/");
      strcat(path_copy, win->dir_list[win->position]->d_name);
      if (access(path_copy, R_OK) == 0) {
        tmp_len = scandir(path_copy, &tmp_dir_list, 0, alphasort);
        if (tmp_len != -1) {
          PathAddDir(win->path, win->dir_list[win->position]->d_name);

          *dir_changed_ptr = 1;
        }
        for (int i = 0; i < tmp_len; i++) {
          free(tmp_dir_list[i]);
        }
        free(tmp_dir_list);
      }

    } else if (!strcmp(win->dir_list[win->position]->d_name, "..")) {
      strcpy(path_copy, win->path);
      strcat(path_copy, "/..");
      if (access(path_copy, R_OK) == 0) {
        tmp_len = scandir(path_copy, &tmp_dir_list, 0, alphasort);
        if (tmp_len != -1) {
          PathDeleteDir(win->path);
          *dir_changed_ptr = 1;
        }
        for (int i = 0; i < tmp_len; i++) {
          free(tmp_dir_list[i]);
        }
        free(tmp_dir_list);
      }
    }
    win->position = 0;
  }
}