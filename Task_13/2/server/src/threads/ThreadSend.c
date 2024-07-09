#include "../../../color.h"
#include "../../server.h"

void *ThreadSend() {
  char send_msg[4];
  unsigned int priority;
  int res = 0;
  char message[MESSAGE_PACK_LEN];

  printf(GREEN "SENDER THREAD HAS BEEN CREATED\n" END_COLOR);

  while (1) {
    if ((res = mq_receive(mqdes_send, send_msg, 4, &priority)) != -1) {
      pthread_mutex_lock(&m1);
      for (int u_i = 0; u_i < USERS_MAX; u_i++) {
        if (user_list[u_i].last_message_index != -1) {
          for (int h_i = 0; h_i < history_index; h_i++) {
            strncpy(message, chat_history[h_i].username, USERNAME_LEN);
            strncat(message, "%%", 1);
            strncpy(message, chat_history[h_i].datetime, DATE_TIME_SIZE);
            strncat(message, "%%", 1);
            strncpy(message, chat_history[h_i].message, CL_MESSAGE_LEN);
            if (mq_send(user_list[u_i].mqdes_client, message, MESSAGE_PACK_LEN,
                        1) == -1) {
              printf(RED "ERROR WHILE SENDING TO CLIENT: %s\n" END_COLOR,
                     strerror(errno));
              sleep(2);
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
}