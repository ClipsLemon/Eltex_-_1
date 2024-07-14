#include "../color.h"
#include "client.h"

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
int shutdown;

int main() {
  initscr();
  start_color();
  noecho();
  cbreak();
  curs_set(FALSE);

  shutdown = 0;

  int scr_row = getmaxy(stdscr);
  int scr_col = getmaxx(stdscr);
  // 2/3 экрана под чат
  int chat_row = scr_row - scr_row / 3;
  int chat_col = scr_col - scr_col / 4;

  int text_row = scr_row / 3;
  int text_col = scr_col - scr_col / 4;

  // инициализация структуры контроллер
  Controller info;
  info.log_file = fopen("log_file.txt", "w");

  info.win_chat_field = newwin(chat_row, chat_col, 0, 0);
  info.win_text_field = newwin(text_row, text_col, chat_row, 0);
  info.win_users_field = newwin(scr_row, scr_col / 4, 0, chat_col);

  box(info.win_chat_field, 0, 0);
  box(info.win_text_field, 0, 0);
  box(info.win_users_field, 0, 0);

  wrefresh(info.win_chat_field);
  wrefresh(info.win_text_field);
  wrefresh(info.win_users_field);

  //  создаем общую память
  int shm_id = shm_open("messenger", DEFAULT_OFLGAS, DEFAULT_MODE);
  if (shm_id == -1) {
    printf("ERROR shm_open: %s", strerror(errno));
    exit(1);
  }
  if (ftruncate(shm_id, sizeof(Chat)) == -1) {
    printf("ERROR ftruncate: %s", strerror(errno));
    exit(1);
  }

  info.chat = (Chat *)mmap(NULL, sizeof(Chat), PROT_READ | PROT_WRITE,
                           MAP_SHARED, shm_id, 0);
  if (info.chat == (void *)-1) {
    printf("ERROR mmap: %s", strerror(errno));
    exit(1);
  }

  // создаем семафор на отправку сообщений
  info.send_message = sem_open(SEM_SEND_MESS, O_RDWR, DEFAULT_MODE, 0);
  if (info.send_message == SEM_FAILED) {
    printf("SEMAPHORE ERROR: %s\n", strerror(errno));
    exit(0);
  }
  sem_post(info.send_message);

  sem_t *shutdown = sem_open(SEM_NAME, DEFAULT_OFLGAS);

  Login(&info);
  clear();

  pthread_t send_message_thread;
  pthread_t print_chat_thread;
  pthread_t find_users_thread;

  pthread_create(&send_message_thread, NULL, ThreadSendMessage, &info);
  pthread_create(&print_chat_thread, NULL, ThreadPrintChat, &info);
  pthread_create(&find_users_thread, NULL, ThreadFindUsers, &info);

  pthread_join(send_message_thread, NULL);
  pthread_join(print_chat_thread, NULL);
  pthread_join(find_users_thread, NULL);

  sem_close(shutdown);

  // в эту очередь функции направляют флаги обновления конкретного экрана, если
  // это нужно

  delwin(info.win_chat_field);
  delwin(info.win_text_field);
  delwin(info.win_users_field);

  endwin();
  return 0;
}