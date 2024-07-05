#include "../main.h"

void GetInput(struct Directory *dir, int *ch, int *dir_changed,
              int *current_window) {

  *ch = wgetch(dir->win);

  switch (*ch) {
  case KEY_DOWN:
    if (dir->position == dir->dir_len - 1) {
      dir->position = 0;
      // опускаем указатель ниже если иначе
    } else {
      dir->position++;
    }
    break;
  case KEY_UP:
    // переходим в конец директории, если нажали вверх в начале
    if (dir->position == 0) {
      dir->position = dir->dir_len - 1;
      // опускаем указатель выше если иначе
    } else {
      dir->position--;
    }
    break;
  case '\n':
    // если пользователь переходит в дирекотрию
    if (dir->dir_list[dir->position]->d_type == 4 ||
        dir->dir_list[dir->position]->d_type == 10) {
      ChangeDir(dir, dir_changed);
    } else {
      OpenFile(dir, dir_changed, *current_window);
    }
    break;
  case '\t':
    *dir_changed = 1;
    *current_window =
        *current_window == LEFT_WINDOW ? RIGHT_WINDOW : LEFT_WINDOW;
    break;
  case KEY_F(1):
    *dir_changed = 1;
    PrintHelp();
    break;
  case KEY_F(2):
    *dir_changed = 1;
    *current_window =
        *current_window == LEFT_WINDOW ? RIGHT_WINDOW : LEFT_WINDOW;
    break;
  case KEY_F(3):
    *dir_changed = 1;
    FindFile(dir);
    break;
  case KEY_F(4):
    *dir_changed = 1;
    Create(dir->path);
    break;
  case KEY_F(5):
    *dir_changed = 1;
    RemoveFile(dir);
    break;
  }
}