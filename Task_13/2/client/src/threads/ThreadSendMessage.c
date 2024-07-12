#include "../../client.h"

void *ThreadSendMessage(void *arg) {
  Controller *info = (Controller *)arg;
  int res = 0;
  char error[1024];
  Message message;
  memset(&message, 0, sizeof(message));
  echo();

  while (1) {
    mvwgetnstr(info->win_text_field, 2, 2, message.message, CL_MESSAGE_LEN);
    if (strncmp(message.message, "/exit", 5) == 0) {
      break;
    }

    pthread_mutex_lock(&m1);
    wclear(info->win_text_field);
    box(info->win_text_field, 0, 0);
    wrefresh(info->win_text_field);

    RemoveNewLineSymbol(message.message);
    CreateMessage(&message, info->client_inf.username);
    if ((res = mq_send(info->mqdes_cl_message, (char *)&message,
                       sizeof(message), 1)) == -1) {
      sprintf(error, "THREAD RECEIVE ERROR: %s", strerror(errno));
      fputs(error, info->log_file);
    }

    memset(message.datetime, 0, DATE_TIME_SIZE);
    memset(message.username, 0, USERNAME_LEN + 1);
    memset(message.message, 0, CL_MESSAGE_LEN);
    pthread_mutex_unlock(&m1);
  }
  noecho();
  return NULL;
}