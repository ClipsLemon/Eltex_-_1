#include "../../../color.h"
#include "../../client.h"
/*
Принимает информацию от сервера в поток SEND_QUEUE
*/
void *ThreadReceive(void *arg) {
  const mqd_t *mqdes_server_msg = (const mqd_t *)arg;

  // сообщение которое пришло пользователю
  char message[MESSAGE_PACK_LEN];
  unsigned int msg_priority = 0;
  // результат считывания сообщения
  ssize_t res = 0;

  printf(GREEN "RECEIVE THREAD HAS BEEN CREATED\n" END_COLOR);
  while (1) {
    // смотрим
    if ((res = mq_receive(*mqdes_server_msg, message, MESSAGE_PACK_LEN,
                          &msg_priority)) != -1) {
      ParseServerMessage(message);
      if (message[1] == '!') {
        break;
      }

    } else if (res == -1) {
      printf(RED "ERROR RECEIVE: can\'t read from queue %s\n" END_COLOR,
             SEND_QUEUE);
      printf(RED "%s\n" END_COLOR, strerror(errno));
    }
  }
  for (int i = 0; i < history_index; i++) {
    printf(BLUE "№ %d\n", i);
    printf("\'%s\'\n", chat_history[i].username);
    printf("\'%s\'\n", chat_history[i].datetime);
    printf("\'%s\'\n" END_COLOR, chat_history[i].message);
  }

  printf(GREEN "RECEIVE THREAD HAS BEEN KILLED\n" END_COLOR);
  return NULL;
}
