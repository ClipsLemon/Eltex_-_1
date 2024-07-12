#include "../../client.h"

void PrintUsersList(Controller *info) {
  wclear(info->win_users_field);
  box(info->win_users_field, 0, 0);
  for (int i = 0, l = 0; i < USERS_MAX; i++) {
    if (info->user_list[i][0] != '\0') {
      mvwprintw(info->win_users_field, l + 1, 1, "%s", info->user_list[i]);
      l++;
    }
  }
  wrefresh(info->win_users_field);
}