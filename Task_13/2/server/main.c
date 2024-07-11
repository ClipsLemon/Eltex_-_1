#include "../color.h"
#include "server.h"

User user_list[USERS_MAX];
pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
Message chat_history[HISTORY_LEN];

int history_index = 0;

mqd_t mqdes_service;
mqd_t mqdes_cl_message;

mqd_t mqdes_send;
mqd_t mqdes_shutdown;

int main() {
  // start server initialization
  // атрибуты

  mq_unlink(SERVICE_QUEUE);
  mq_unlink(CLIENT_MS_QUEUE);
  mq_unlink(SEND_QUEUE);

  mqdes_service = QueueOpen(SERVICE_QUEUE, DEFAULT_OFLGAS, DEFAULT_MODE,
                            SERVICE_MESSAGE_LEN, MAX_SERVICE_MESSAGE);
  mqdes_cl_message = QueueOpen(CLIENT_MS_QUEUE, DEFAULT_OFLGAS, DEFAULT_MODE,
                               MESSAGE_PACK_LEN, MAX_CL_MESSAGE);
  mqdes_send =
      QueueOpen(SEND_QUEUE, DEFAULT_OFLGAS, DEFAULT_MODE, 4, MAX_CL_MESSAGE);

  // QueueClose(mqdes_service, SERVICE_QUEUE);
  // QueueClose(mqdes_cl_message, CLIENT_MS_QUEUE);
  // QueueClose(mqdes_send, SEND_QUEUE);

  // очередь нужна для выключения
  mqdes_shutdown =
      QueueOpen(SHUTDOWN_SERVER_QUEUE, DEFAULT_OFLGAS, DEFAULT_MODE, 2, 1);

  // запускаем потоки сервера
  pthread_t service_thread;
  pthread_t cl_message_thread;
  pthread_t send_thread;
  pthread_t shutdown_thread;

  // зануляем всех клиентов
  for (int i = 0; i < USERS_MAX; i++) {
    user_list[i].last_message_index = -1;
  }

  pthread_create(&service_thread, NULL, ThreadServiceReceive, &mqdes_service);
  pthread_create(&cl_message_thread, NULL, ThreadClMessage, NULL);
  pthread_create(&send_thread, NULL, ThreadSend, NULL);
  pthread_create(&shutdown_thread, NULL, ThreadShutdown, NULL);

  // end server initialization

  int a = 0;
  char server_shutdown[2];
  printf(BLUE "LOG: server is running\n" END_COLOR);
  // ждем сообщения о выключении сервера
  mq_receive(mqdes_shutdown, server_shutdown, 2, &a);

  // отключаем все очереди, чтобы они не повисали в системе
  // TODO: разослать во очереди сообщений что сервер выключается и завершить все
  // потоки
  for (int i = 0; i < USERS_MAX; i++) {
    // если пользователь был создан, то чистим очередь
    if (user_list[i].last_message_index != -1) {
      QueueClose(user_list[i].mqdes_client, user_list[i].queue_name);
    }
  }

  QueueClose(mqdes_service, SERVICE_QUEUE);
  QueueClose(mqdes_cl_message, CLIENT_MS_QUEUE);
  QueueClose(mqdes_send, SEND_QUEUE);
  QueueClose(mqdes_shutdown, SHUTDOWN_SERVER_QUEUE);

  printf(BLUE "LOG: server is closed\n" END_COLOR);

  return 0;
}