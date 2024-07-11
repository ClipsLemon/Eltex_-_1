#include "../color.h"
#include "client.h"

mqd_t mqdes_cl_message;
mqd_t mqdes_service;
mqd_t mqdes_send;

Message chat_history[HISTORY_LEN];

int history_index = 0;

int main() {
  mqdes_service = QueueConnect(SERVICE_QUEUE, O_WRONLY, DEFAULT_MODE,
                               SERVICE_MESSAGE_LEN, MAX_SERVICE_MESSAGE);
  mqdes_cl_message = QueueConnect(CLIENT_MS_QUEUE, O_WRONLY, DEFAULT_MODE,
                                  MESSAGE_PACK_LEN, MAX_CL_MESSAGE);
  mqdes_send =
      QueueConnect(SEND_QUEUE, DEFAULT_OFLGAS, DEFAULT_MODE, 4, MAX_CL_MESSAGE);

  char username[SERVICE_MESSAGE_LEN];
  username[0] = 'c';
  char queue_name[USERNAME_LEN + 1];
  // пишем начиная с первого
  GetName(username);
  strcpy(queue_name, username);
  queue_name[0] = '/';
  mq_send(mqdes_service, username, SERVICE_MESSAGE_LEN, 1);

  char message[MESSAGE_PACK_LEN];
  CreateMessage(message, username);

  mq_send(mqdes_cl_message, message, MESSAGE_PACK_LEN, 1);

  mqd_t mqdes_server_msg =
      QueueConnect(queue_name, DEFAULT_OFLGAS, DEFAULT_MODE, MESSAGE_PACK_LEN,
                   MAX_CL_MESSAGE);

  pthread_t send_receive_thread;
  pthread_create(&send_receive_thread, NULL, ThreadReceive, &mqdes_server_msg);

  pthread_join(send_receive_thread, NULL);
  // username[0] = 'd';
  // mq_send(mqdes_service, username, SERVICE_MESSAGE_LEN, 1);

  QueueDisconnect(mqdes_service, SERVICE_QUEUE);
  QueueDisconnect(mqdes_cl_message, CLIENT_MS_QUEUE);
  QueueDisconnect(mqdes_server_msg, queue_name);

  return 0;
}