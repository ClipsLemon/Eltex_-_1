#include "../color.h"
#include "client.h"

FILE *log_file;

mqd_t mqdes_cl_message;
mqd_t mqdes_service;
mqd_t mqdes_send;
mqd_t mqdes_upd_wind;

WINDOW *win_chat_field;
WINDOW *win_text_field;

Message chat_history[HISTORY_LEN];
pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
int history_index = 0;

int main() {
  initscr();
  start_color();
  noecho();
  cbreak();
  curs_set(FALSE);
  log_file = fopen("log_file.txt", "w");

  int scr_row = getmaxy(stdscr);
  int scr_col = getmaxx(stdscr);
  // 2/3 экрана под чат
  int chat_row = scr_row - scr_row / 3;
  int chat_col = scr_col - scr_col / 4;

  int text_row = scr_row / 3;
  int text_col = scr_col - scr_col / 4;

  win_chat_field = newwin(chat_row, chat_col, 0, 0);
  win_text_field = newwin(text_row, text_col, chat_row, 0);

  box(win_chat_field, 0, 0);
  box(win_text_field, 0, 0);

  wrefresh(win_chat_field);
  wrefresh(win_text_field);

  mqdes_service = QueueConnect(SERVICE_QUEUE, O_WRONLY, DEFAULT_MODE,
                               SERVICE_MESSAGE_LEN, MAX_SERVICE_MESSAGE);

  mqdes_cl_message = QueueConnect(CLIENT_MS_QUEUE, O_WRONLY, DEFAULT_MODE,
                                  MESSAGE_PACK_LEN, MAX_CL_MESSAGE);
  mqdes_send =
      QueueConnect(SEND_QUEUE, DEFAULT_OFLGAS, DEFAULT_MODE, 4, MAX_CL_MESSAGE);

  pthread_t client_serv_thread;
  pthread_create(&client_serv_thread, NULL, ThreadSendServiceMessage, NULL);

  // в эту очередь функции направляют флаги обновления конкретного экрана, если
  // это нужно
  mqdes_upd_wind = QueueConnect(UPD_WIN_QUEUE, O_RDWR, DEFAULT_MODE, 2, 25);

  pthread_join(client_serv_thread, NULL);

  QueueDisconnect(mqdes_service, SERVICE_QUEUE);
  QueueDisconnect(mqdes_cl_message, CLIENT_MS_QUEUE);

  endwin();
  return 0;
}