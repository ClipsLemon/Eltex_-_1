#include "server.h"

int main() {
  // start server initialization
  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = MAX_MESSAGE;
  attr.mq_msgsize = MESSAGE_SIZE;
  attr.mq_curmsgs = 0;

  mqd_t mqdes_service =
      QueueOpen(SERVICE_QUEUE, DEFAULT_OFLGAS, DEFAULT_MODE, &attr);
  mqd_t mqdes_cl_message =
      QueueOpen(CLIENT_MS_QUEUE, DEFAULT_OFLGAS, DEFAULT_MODE, &attr);
  mqd_t mqdes_send = QueueOpen(SEND_QUEUE, DEFAULT_OFLGAS, DEFAULT_MODE, &attr);
  // end server initialization

  // создаем список пользователей
  User user_list[USERS_MAX];
  Message chat_history[HISTORY_LEN];

  QueueClose(mqdes_service, SERVICE_QUEUE);
  QueueClose(mqdes_cl_message, CLIENT_MS_QUEUE);
  QueueClose(mqdes_send, SEND_QUEUE);

  return 0;
}