#include "../../client.h"

void Login(Controller *info) {
  WINDOW *login_window = newwin(4, 35, 10, 10);

  box(login_window, 0, 0);
  echo();
  mvwprintw(login_window, 1, 1, "Etner nickname (<=%d symbols):", USERNAME_LEN);
  mvwgetnstr(login_window, 2, 1, info->username, USERNAME_LEN);
  wrefresh(login_window);
  noecho();

  RemoveNewLineSymbol(info->username);
  strncpy(info->chat->chat_history[info->chat->history_index].username,
          info->username, USERNAME_LEN);
  strncpy(info->chat->chat_history[info->chat->history_index].message,
          "logged in", 10);
  info->chat->history_index++;

  wclear(login_window);
  box(login_window, 0, 0);
  mvwprintw(login_window, 1, 1, "Greetings %s!", info->username);
  wrefresh(login_window);
  sleep(1);
  delwin(login_window);
}