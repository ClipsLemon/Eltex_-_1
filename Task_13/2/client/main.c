#include "../color.h"
#include "client.h"

mqd_t mqdes_cl_message;

int main() {
  mqd_t mqdes_service = QueueConnect(SERVICE_QUEUE, O_WRONLY, DEFAULT_MODE,
                                     SERVICE_MESSAGE_LEN, MAX_SERVICE_MESSAGE);
  mqdes_cl_message = QueueConnect(CLIENT_MS_QUEUE, O_WRONLY, DEFAULT_MODE,
                                  MESSAGE_PACK_LEN, MAX_CL_MESSAGE);

  char username[SERVICE_MESSAGE_LEN];
  username[0] = 'c';
  // пишем начиная с первого
  GetName(username);
  mq_send(mqdes_service, username, SERVICE_MESSAGE_LEN, 1);

  char message[MESSAGE_PACK_LEN];
  CreateMessage(message, username);

  mq_send(mqdes_cl_message, message, MESSAGE_PACK_LEN, 1);

  username[0] = 'd';
  mq_send(mqdes_service, username, SERVICE_MESSAGE_LEN, 1);

  QueueDisconnect(mqdes_service, SERVICE_QUEUE);
  QueueDisconnect(mqdes_cl_message, CLIENT_MS_QUEUE);

  return 0;
}