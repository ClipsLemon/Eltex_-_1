#include "../../client.h"

void PrintChat(Controller *info) {
  wclear(info->win_chat_field);
  wrefresh(info->win_chat_field);
  int start = 0;
  box(info->win_chat_field, 0, 0);
  wrefresh(info->win_chat_field);
  if (info->history_index - getmaxy(info->win_chat_field) + 2 > 0) {
    start = info->history_index - getmaxy(info->win_chat_field) + 3;
  }

  for (int i = start, l = 1;
       i < info->history_index && l < getmaxy(info->win_chat_field) - 1;
       i++, l++) {
    mvwprintw(info->win_chat_field, l, 2, "%-20s | %-17s | %s",
              &(info->chat_history[i].username), info->chat_history[i].datetime,
              info->chat_history[i].message);
  }
  wrefresh(info->win_chat_field);
}