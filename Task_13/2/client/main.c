#include "../color.h"
#include "client.h"

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;

int main() {
  initscr();
  start_color();
  noecho();
  cbreak();
  curs_set(FALSE);

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

  memset(&info.chat_history, 0, sizeof(info.chat_history));
  memset(&info.user_list, 0, sizeof(info.user_list));

  info.win_chat_field = newwin(chat_row, chat_col, 0, 0);

  info.win_text_field = newwin(text_row, text_col, chat_row, 0);
  info.win_users_field = newwin(scr_row, scr_col / 4, 0, chat_col);

  info.mqdes_service =
      QueueConnect(SERVICE_QUEUE, O_WRONLY, DEFAULT_MODE, SERVICE_MESSAGE_LEN,
                   MAX_SERVICE_MESSAGE, info.log_file);

  info.mqdes_cl_message =
      QueueConnect(CLIENT_MS_QUEUE, O_WRONLY, DEFAULT_MODE, MESSAGE_PACK_LEN,
                   MAX_CL_MESSAGE, info.log_file);
  info.mqdes_send = QueueConnect(SEND_QUEUE, DEFAULT_OFLGAS, DEFAULT_MODE, 4,
                                 MAX_CL_MESSAGE, info.log_file);

  box(info.win_chat_field, 0, 0);
  box(info.win_text_field, 0, 0);
  box(info.win_users_field, 0, 0);

  wrefresh(info.win_chat_field);
  wrefresh(info.win_text_field);
  wrefresh(info.win_users_field);

  pthread_t client_serv_thread;
  pthread_create(&client_serv_thread, NULL, ThreadSendServiceMessage,
                 (void *)&info);

  // в эту очередь функции направляют флаги обновления конкретного экрана, если
  // это нужно

  pthread_join(client_serv_thread, NULL);

  QueueDisconnect(info.mqdes_service, SERVICE_QUEUE);
  QueueDisconnect(info.mqdes_cl_message, CLIENT_MS_QUEUE);

  delwin(info.win_chat_field);
  delwin(info.win_text_field);
  delwin(info.win_users_field);

  endwin();
  return 0;
}