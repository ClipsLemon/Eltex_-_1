#include "../../../color.h"
#include "../../client.h"

void Login(char *username) {
  WINDOW *login_window = newwin(4, 35, 10, 10);

  box(login_window, 0, 0);
  echo();
  mvwprintw(login_window, 1, 1, "Etner nickname (<=%d symbols):\n",
            USERNAME_LEN);
  wgetnstr(login_window, &username[1], USERNAME_LEN);
  wrefresh(login_window);
  noecho();

  RemoveNewLineSymbol(username);

  wclear(login_window);
  box(login_window, 0, 0);
  mvwprintw(login_window, 1, 1, "Greetings %s!\n", &username[1]);
  wrefresh(login_window);
  sleep(1);
  PrintChat();
  delwin(login_window);
}