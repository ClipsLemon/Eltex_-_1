#include "../../client.h"

void PrintChat() {
  wclear(win_chat_field);
  wrefresh(win_chat_field);

  box(win_chat_field, 1, 1);
  wrefresh(win_chat_field);
  for (int i = 0; i < (getmaxy(win_chat_field) - 2) / 2 && i < history_index;
       i++) {
    mvwprintw(win_chat_field, i + 1, 2, "%-20s | %-17s | %s",
              &chat_history[i].username[1], chat_history[i].datetime,
              chat_history[i].message);
  }
  wrefresh(win_chat_field);
}