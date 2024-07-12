#include "../../../color.h"
#include "../../server.h"

void *ThreadSend(void *arg) {
  Controller *info = (Controller *)arg;
  char send_msg[4];
  unsigned int priority;
  int res = 0;

  printf(GREEN "SENDER THREAD HAS BEEN CREATED\n" END_COLOR);

  while (1) {
    if ((res = mq_receive(info->mqdes_send, send_msg, 4, &priority)) != -1) {
      pthread_mutex_lock(&m1);
      printf(BLUE "SENDING...\n" END_COLOR);
      for (int u_i = 0; u_i < USERS_MAX; u_i++) {
        if (info->user_list[u_i].last_message_index != -1) {
          for (int h_i = 0; h_i < info->history_index; h_i++) {
            if (h_i >= info->user_list[u_i].last_message_index) {
              if (mq_send(info->user_list[u_i].mqdes_client,
                          (char *)(&info->chat_history[h_i]), MESSAGE_PACK_LEN,
                          1) == -1) {
                printf(RED "ERROR WHILE SENDING TO CLIENT: %s\n" END_COLOR,
                       strerror(errno));
                sleep(2);
              }
              info->user_list[u_i].last_message_index++;
            }
          }
        }
      }
      printf(BLUE "MESSAGES HAS BEEN SEND\n" END_COLOR);
      pthread_mutex_unlock(&m1);
    } else {
      printf(RED "ERROR SEND RECEIVE: can\'t read from queue %s\n" END_COLOR,
             SEND_QUEUE);
      sleep(2);

      printf(RED "%s\n" END_COLOR, strerror(errno));
    }
  }
  return NULL;
}