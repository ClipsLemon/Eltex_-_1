#include "../../client.h"

void PrintChat() {
  wclear(win_chat_field);
  wrefresh(win_chat_field);
  int start = 0;
  box(win_chat_field, 0, 0);
  wrefresh(win_chat_field);
  if (history_index - getmaxy(win_chat_field) - 2 > 0) {
    start = history_index - getmaxy(win_chat_field) + 2;
  }

  for (int i = start; i < history_index; i++) {
    mvwprintw(win_chat_field, 1 + i - start, 2, "%-20s | %-17s | %s",
              &chat_history[i].username, chat_history[i].datetime,
              chat_history[i].message);
  }
  wrefresh(win_chat_field);
}