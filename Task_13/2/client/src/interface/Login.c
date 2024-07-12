#include "../../client.h"

void Login(Controller *info) {
  WINDOW *login_window = newwin(4, 35, 10, 10);

  box(login_window, 0, 0);
  echo();
  mvwprintw(login_window, 1, 1, "Etner nickname (<=%d symbols):", USERNAME_LEN);
  mvwgetnstr(login_window, 2, 1, &(info->client_inf.username[1]), USERNAME_LEN);
  wrefresh(login_window);
  noecho();

  RemoveNewLineSymbol(info->client_inf.username);

  wclear(login_window);
  box(login_window, 0, 0);
  mvwprintw(login_window, 1, 1, "Greetings %s!",
            &(info->client_inf.username[1]));
  wrefresh(login_window);
  sleep(1);
  delwin(login_window);

  PrintChat(info);
  wclear(info->win_text_field);
  box(info->win_text_field, 0, 0);
  wrefresh(info->win_text_field);
}