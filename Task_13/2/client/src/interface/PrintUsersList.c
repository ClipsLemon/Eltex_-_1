#include "../../client.h"

void PrintUsersList() {
  wclear(win_users_field);
  box(win_users_field, 0, 0);
  for (int i = 0, l = 0; i < USERS_MAX; i++) {
    if (user_list[i][0] != '\0') {
      mvwprintw(win_users_field, l + 1, 1, "%s", user_list[i]);
      l++;
    }
  }
  wrefresh(win_users_field);
}