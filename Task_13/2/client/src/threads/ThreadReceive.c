#include "../../../color.h"
#include "../../client.h"
/*
Принимает информацию от сервера в потоке клиента
*/
void *ThreadReceive(void *arg) {
  const mqd_t *mqdes_server_msg = (const mqd_t *)arg;

  // сообщение которое пришло пользователю
  Message *tmp_message;
  unsigned int msg_priority = 0;
  // результат считывания сообщения
  ssize_t res = 0;

  fputs(GREEN "RECEIVE THREAD HAS BEEN CREATED\n" END_COLOR, log_file);
  while (1) {
    // смотрим
    if ((res =
             mq_receive(*mqdes_server_msg, (char *)&chat_history[history_index],
                        MESSAGE_PACK_LEN, &msg_priority)) != -1) {
      pthread_mutex_lock(&m1);
      history_index++;
      PrintChat();
      pthread_mutex_unlock(&m1);
    } else if (res == -1) {
      // printf(RED "ERROR RECEIVE: can\'t read from client queue\n" END_COLOR);
      // printf(RED "%s\n" END_COLOR, strerror(errno));
      sleep(2);
    }
    // for (int i = 0; i < history_index; i++) {
    //   printf(BLUE "№ %d\n", i);
    //   printf("\'%s\'\n", chat_history[i].username);
    //   printf("\'%s\'\n", chat_history[i].datetime);
    //   printf("\'%s\'\n" END_COLOR, chat_history[i].message);
    // }
  }

  // printf(GREEN "RECEIVE THREAD HAS BEEN KILLED\n" END_COLOR);
  return NULL;
}
