#include "../../main.h"

void PrintManager(WINDOW *footer, struct Directory *window_list,
                  char *home_path) {
  // ввод пользователя
  int ch = ' ';
  int current_window = LEFT_WINDOW;
  int dir_changed = 0;

  while (ch != 'q') {
    // если мы сменили директорию, то путь надо отрисовать заново и обновить
    // список объектов директории
    if (dir_changed) {
      // чистим окно (что бы избежать наложения длинных имен файлов на короткие)
      wclear(window_list[LEFT_WINDOW].win);
      wclear(window_list[RIGHT_WINDOW].win);
      // выводим пути
      PrintPath(&window_list[LEFT_WINDOW], home_path);
      PrintPath(&window_list[RIGHT_WINDOW], home_path);
      // чистим память
      FreeDirList(&window_list[LEFT_WINDOW]);
      FreeDirList(&window_list[RIGHT_WINDOW]);
      // заново сканируем директорию
      window_list[LEFT_WINDOW].dir_len =
          scandir(window_list[LEFT_WINDOW].path,
                  &(window_list[LEFT_WINDOW].dir_list), 0, alphasort);
      window_list[RIGHT_WINDOW].dir_len =
          scandir(window_list[RIGHT_WINDOW].path,
                  &(window_list[RIGHT_WINDOW].dir_list), 0, alphasort);
      dir_changed = 0;
    }

    PrintDir(&(window_list[RIGHT_WINDOW]), home_path);
    PrintDir(&(window_list[LEFT_WINDOW]), home_path);
    PrintFooter(footer);
    // берем пользовательский ввод
    GetInput(&(window_list[current_window]), &ch, &dir_changed,
             &current_window);
  }
}