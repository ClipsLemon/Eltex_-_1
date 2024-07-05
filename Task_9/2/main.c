#include "main.h"

struct Directory *window_list_ptr;
char *home_path_ptr;
const char *text_editors[] = {"nano", "vim", "emacs", "gedit",
                              "kate", "hx",  NULL};

int text_editor_count = 0;
char **editor_list;

int main(void) {

  // считаем количество доступных редакторов
  for (int i = 0; text_editors[i] != NULL; i++) {
    if (is_program_installed(text_editors[i])) {
      text_editor_count++;
    }
  }
  editor_list = calloc(text_editor_count, sizeof(char *));
  for (int i = 0, l = 0; text_editors[i] != NULL; i++) {
    if (is_program_installed(text_editors[i])) {
      editor_list[l] = calloc(256, sizeof(char));
      ClearString(editor_list[l], 256);
      strcat(editor_list[l], text_editors[i]);
      l++;
    }
  }

  // переменная домашнего каталога, например: /home/sancepas
  char home_path[HOME_PATH_LEN];
  getcwd(home_path, HOME_PATH_LEN);
  for (int i = 1; i < HOME_PATH_LEN; i++) {
    if (home_path[i] == '/') {
      home_path[i] = '\0';
      break;
    }
  }

  struct Directory window_list[2];

  window_list_ptr = window_list;
  home_path_ptr = home_path;

  initscr();
  start_color();
  noecho();
  cbreak();

  ESCDELAY = 0;
  curs_set(FALSE);

  int row = getmaxy(stdscr);
  int col = getmaxx(stdscr);
  // иниция окна
  InitWin(&window_list[0], row - 1, col / 2 + 1, 0, 0);
  InitWin(&window_list[1], row - 1, col / 2, 0, col / 2 + 1);

  WINDOW *footer = newwin(1, col, row - 1, 0);
  keypad(window_list[1].win, true);

  // создание цвета для фона
  init_color(COLOR_BACKGROUND, 0, 255, 251);

  // Инициирование пары цветов для файлов
  init_pair(1, COLOR_WHITE, COLOR_CYAN);
  // Инициирование пары цветов для каталогов
  init_pair(2, COLOR_BLACK, COLOR_GREEN);
  // задаем цветовую пару для заднего фона
  init_pair(3, COLOR_WHITE, COLOR_BACKGROUND);
  init_pair(4, COLOR_BLACK, COLOR_WHITE);
  init_pair(5, COLOR_WHITE, COLOR_RED);
  // задать цвета
  wbkgd(window_list[LEFT_WINDOW].win, COLOR_PAIR(3));
  wbkgd(window_list[RIGHT_WINDOW].win, COLOR_PAIR(3));
  wbkgd(footer, COLOR_PAIR(4));

  // сначала надо один раз отрисовать все вручную, что бы при входе пользователь
  // видел менеджер
  PrintPath(&(window_list[LEFT_WINDOW]), home_path);
  PrintPath(&(window_list[RIGHT_WINDOW]), home_path);
  PrintDir(&(window_list[RIGHT_WINDOW]), home_path);

  PrintManager(footer, window_list, home_path);

  // чистим память
  FreeDirList(&window_list[0]);
  FreeDirList(&window_list[1]);
  delwin(window_list[0].win);
  delwin(window_list[1].win);
  delwin(footer);

  for (int i = 0; i < text_editor_count; i++) {
    free(editor_list[i]);
  }
  free(editor_list);

  endwin(); // завершение работы с ncurses

  return 0;
}
