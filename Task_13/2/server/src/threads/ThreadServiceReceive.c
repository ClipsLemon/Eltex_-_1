#include "../../../color.h"
#include "../../server.h"

void *ThreadServiceReceive(void *arg) {
  // const mqd_t *mqdes_service_ptr = (const mqd_t *)mqdes_service;
  // сообщение которое пришло пользователю
  char message[SERVICE_MESSAGE_LEN];
  unsigned int msg_priority = 0;
  // результат считывания сообщения
  ssize_t res = 0;

  printf(GREEN "SERVICE THREAD HAS BEEN CREATED\n" END_COLOR);
  while (1) {
    if ((res = mq_receive(mqdes_service, message, SERVICE_MESSAGE_LEN,
                          &msg_priority)) != -1) {
      pthread_mutex_lock(&m1);

      // блокируем мьютекс на момент редактирования данных
      if (message[0] == CLIENT_CON) {
        printf(BLUE "LOG: client CONNECTED:    %s\n" END_COLOR, message);
        AddClient(message);
        pthread_mutex_unlock(&m1);
      } else if (message[0] == CLIENT_DISC) {
        printf(BLUE "LOG: client DISCONNECTED: %s\n" END_COLOR, message);
        DeleteClient(message);
      }   
      pthread_mutex_unlock(&m1);

    } else if (res == -1) {
      printf(RED "ERROR SERVICE RECEIVE: can\'t read from queue %s\n" END_COLOR,
             SERVICE_QUEUE);
      printf(RED "%s\n" END_COLOR, strerror(errno));
    }
  }
  printf(GREEN "SERVICE THREAD HAS BEEN KILLED\n" END_COLOR);
  return NULL;
}