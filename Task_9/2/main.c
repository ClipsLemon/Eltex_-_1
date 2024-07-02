#include "main.h"

int main(void) {
  // переменная домашнего каталога, например: /home/sancepas
  char home_path[HOME_PATH_LEN];
  getcwd(home_path, HOME_PATH_LEN);
  for (int i = 1; i < HOME_PATH_LEN; i++) {
    if (home_path[i] == '/') {
      home_path[i] = '\0';
      break;
    }
  }

  // ввод пользователя
  int ch = ' ';
  int current_window = LEFT_WINDOW;
  int dir_changed = 0;

  struct MyWin window_list[2];

  initscr();
  start_color();
  noecho();
  cbreak();
  curs_set(FALSE);

  int row = getmaxy(stdscr);
  int col = getmaxx(stdscr);

  // иниция окна
  InitWin(&window_list[0], row, col, 0, 0);
  InitWin(&window_list[1], row, col, 0, col / 2);

  // создание цвета для фона
  init_color(COLOR_BACKGROUND, 0, 255, 251);

  // Инициирование пары цветов для файлов
  init_pair(1, COLOR_WHITE, COLOR_CYAN);
  // Инициирование пары цветов для каталогов
  init_pair(2, COLOR_BLACK, COLOR_GREEN);
  // задаем цветовую пару для заднего фона
  init_pair(3, COLOR_WHITE, COLOR_BACKGROUND);

  // задать цвета
  wbkgd(window_list[LEFT_WINDOW].win, COLOR_PAIR(3));
  wbkgd(window_list[RIGHT_WINDOW].win, COLOR_PAIR(3));

  // сначала надо один раз отрисовать все вручную, что бы при входе пользователь
  // видел менеджер
  PrintPath(&window_list[LEFT_WINDOW], home_path);
  PrintPath(&window_list[RIGHT_WINDOW], home_path);
  PrintDir(&(window_list[RIGHT_WINDOW]), home_path);

  while (ch != 'q') {
    // если мы сменили директорию, то путь надо отрисовать заново и обновить
    // список объектов директории
    if (dir_changed) {
      // чистим окно (что бы избежать наложения длинных имен файлов на короткие)
      wclear(window_list[current_window].win);
      // выводим пути
      PrintPath(&window_list[current_window], home_path);
      // чистим память
      FreeDirList(&window_list[current_window]);
      // заново сканируем директорию
      window_list[current_window].dir_len =
          scandir(window_list[current_window].path,
                  &(window_list[current_window].dir_list), 0, alphasort);
      dir_changed = 0;
    }

    PrintDir(&(window_list[current_window]), home_path);
    // берем пользовательский ввод
    ch = wgetch(window_list[current_window].win);

    if (ch == KEY_DOWN) {
      // возвращаемся в начало директории, если дошли до последнего элемента
      if (window_list[current_window].position ==
          window_list[current_window].dir_len - 1) {
        window_list[current_window].position = 0;
        // опускаем указатель ниже если иначе
      } else {
        window_list[current_window].position++;
      }
    } else if (ch == KEY_UP) {
      // переходим в конец директории, если нажали вверх в начале
      if (window_list[current_window].position == 0) {
        window_list[current_window].position =
            window_list[current_window].dir_len - 1;
        // опускаем указатель выше если иначе
      } else {
        window_list[current_window].position--;
      }
      // если пользователь решил перейти в папку, то меняем дриекторию
    } else if (ch == '\n') {
      ChangeDir(&window_list[current_window], &dir_changed, home_path);
    } else if (ch == 'c') {
      dir_changed = 1;
      current_window =
          current_window == LEFT_WINDOW ? RIGHT_WINDOW : LEFT_WINDOW;
    }
  }

  // чистим память
  FreeDirList(&window_list[0]);
  FreeDirList(&window_list[1]);
  delwin(window_list[0].win);
  delwin(window_list[1].win);
  endwin(); // завершение работы с ncurses

  return 0;
}

void InitWin(struct MyWin *win, int row, int col, int y, int x) {
  win->win = newwin(row, col / 2, y, x);
  win->position = 0;
  getcwd(win->path, PATH_LEN);
  win->dir_len = scandir(win->path, &(win->dir_list), 0, alphasort);
  keypad(win->win, TRUE);
}

void PrintPath(struct MyWin *win, char *home_path) {
  mvwprintw(win->win, 1, 1, "CURRENT PATH: %-*s", getmaxx(win->win) - 15,
            win->path);
  mvwprintw(win->win, 2, 1, "HOME DIR: %-*s", getmaxx(win->win) - 15,
            home_path);
}

void FreeDirList(struct MyWin *win) {
  for (int i = 0; i < win->dir_len; i++) {
    free(win->dir_list[i]);
  }
  free(win->dir_list);
}