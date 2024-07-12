#include "../../../color.h"
#include "../../client.h"

void *ThreadSendMessage(void *arg) {
  Controller *info = (Controller *)arg;

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
    mq_send(info->mqdes_cl_message, (char *)&message, sizeof(message), 1);

    ClearString(message.datetime, DATE_TIME_SIZE);
    ClearString(message.username, USERNAME_LEN + 1);
    ClearString(message.message, CL_MESSAGE_LEN);
    pthread_mutex_unlock(&m1);
  }
  noecho();
  return NULL;
}