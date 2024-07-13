#include "../../client.h"

void PrintTextField(Controller *info) {
  wclear(info->win_text_field);
  box(info->win_text_field, 0, 0);
  wrefresh(info->win_text_field);
}