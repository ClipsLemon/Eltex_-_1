#include "../../client.h"

void PrintChat(Controller *info) {
  wclear(info->win_chat_field);
  int start = 0;
  box(info->win_chat_field, 0, 0);
  if (info->chat->history_index - getmaxy(info->win_chat_field) + 2 > 0) {
    start = info->chat->history_index - getmaxy(info->win_chat_field) + 3;
  }

  for (int i = start, l = 1;
       i < info->chat->history_index && l < getmaxy(info->win_chat_field) - 1;
       i++, l++) {
    mvwprintw(info->win_chat_field, l, 2, "%-20s | %-17s | %s",
              &(info->chat->chat_history[i].username[1]),
              info->chat->chat_history[i].datetime,
              info->chat->chat_history[i].message);
  }
  wrefresh(info->win_chat_field);
}