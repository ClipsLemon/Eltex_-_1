#include "../../main.h"

void PrintFooter(WINDOW *footer) {
  // размер одного ячейки информации
  // пока что 5 элементов отображается внизу
  int cell_size = getmaxx(footer) / 5;
  mvwprintw(footer, 0, 0, "%-*s|%-*s|%-*s|%-*s|%-*s", cell_size - 1, "F1 Help",
            cell_size - 1, "F2 Set Color", cell_size - 1, "F3 Find File",
            cell_size - 1, "F4 Create F/D", cell_size - 1, "F5 Remove File");
  wrefresh(footer);
}