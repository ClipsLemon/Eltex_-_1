#include "../../../color.h"
#include "../../server.h"

void *ThreadClMessage() {
  printf(GREEN "CLIENT MESSAGES THREAD HAS BEEN CREATED\n" END_COLOR);
  unsigned int priority = 0;
  int res = 0;
  Message message;
  memset(&message, 0, sizeof(message));

  while (1) {
    if ((res = mq_receive(mqdes_cl_message, (char *)&message, MESSAGE_PACK_LEN,
                          &priority)) != -1) {
      pthread_mutex_lock(&m1);
      memcpy(&chat_history[history_index], &message, sizeof(message));
      history_index++;
      mq_send(mqdes_send, "upd", 4, 1);

      // тестовый вывод всей истории после записи сообщения клиента
      printf(BLUE "MESSAGE HISTORY\n" END_COLOR);

      for (int i = 0; i < history_index; i++) {
        printf(BLUE "№ %d\n", i);
        printf("\'%s\'\n", chat_history[i].username);
        printf("\'%s\'\n", chat_history[i].datetime);
        printf("\'%s\'\n" END_COLOR, chat_history[i].message);
      }
      pthread_mutex_unlock(&m1);

    } else {
      printf(RED "ERROR SERVICE RECEIVE: can\'t read from queue %s\n" END_COLOR,
             CLIENT_MS_QUEUE);
      sleep(2);

      printf(RED "%s\n" END_COLOR, strerror(errno));
    }
  }
  return NULL;
}