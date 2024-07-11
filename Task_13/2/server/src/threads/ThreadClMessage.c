#include "../../../color.h"
#include "../../server.h"

void *ThreadClMessage() {
  printf(GREEN "CLIENT MESSAGES THREAD HAS BEEN CREATED\n" END_COLOR);

  while (1) {
    char message[MESSAGE_PACK_LEN];
    unsigned int priority = 0;
    int res = 0;
    if ((res = mq_receive(mqdes_cl_message, message, MESSAGE_PACK_LEN,
                          &priority)) != -1) {
      pthread_mutex_lock(&m1);
      mq_send(mqdes_send, "upd", 4, 1);
      ParseClientMessage(message);
      pthread_mutex_unlock(&m1);

      // тестовый вывод всей истории после записи сообщения клиента
      printf(BLUE "MESSAGE HISTORY\n" END_COLOR);
      pthread_mutex_lock(&m1);

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