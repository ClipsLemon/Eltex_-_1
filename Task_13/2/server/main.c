#include "../color.h"
#include "server.h"

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;

int main() {
  // start server initialization
  // атрибуты
  Controller info;
  info.history_index = 0;
  info.mqdes_service = QueueOpen(SERVICE_QUEUE, DEFAULT_OFLGAS, DEFAULT_MODE,
                                 SERVICE_MESSAGE_LEN, MAX_SERVICE_MESSAGE);
  info.mqdes_cl_message =
      QueueOpen(CLIENT_MS_QUEUE, DEFAULT_OFLGAS, DEFAULT_MODE, MESSAGE_PACK_LEN,
                MAX_CL_MESSAGE);
  info.mqdes_send =
      QueueOpen(SEND_QUEUE, DEFAULT_OFLGAS, DEFAULT_MODE, 4, MAX_CL_MESSAGE);

  // очередь нужна для выключения
  info.mqdes_shutdown =
      QueueOpen(SHUTDOWN_SERVER_QUEUE, DEFAULT_OFLGAS, DEFAULT_MODE, 2, 1);

  // запускаем потоки сервера
  pthread_t service_thread;
  pthread_t cl_message_thread;
  pthread_t send_thread;
  pthread_t shutdown_thread;

  memset(&info.chat_history, 0, sizeof(info.chat_history));
  memset(&info.user_list, 0, sizeof(info.user_list));

  // зануляем всех клиентов
  for (int i = 0; i < USERS_MAX; i++) {
    info.user_list[i].last_message_index = -1;
  }

  pthread_create(&service_thread, NULL, ThreadServiceReceive, &info);
  pthread_create(&cl_message_thread, NULL, ThreadClMessage, &info);
  pthread_create(&send_thread, NULL, ThreadSend, &info);
  pthread_create(&shutdown_thread, NULL, ThreadShutdown, &info);

  // end server initialization

  int a = 0;
  char server_shutdown[2];
  printf(BLUE "LOG: server is running\n" END_COLOR);
  // ждем сообщения о выключении сервера
  mq_receive(info.mqdes_shutdown, server_shutdown, 2, &a);

  // отключаем все очереди, чтобы они не повисали в системе
  // TODO: разослать во очереди сообщений что сервер выключается и завершить все
  // потоки
  for (int i = 0; i < USERS_MAX; i++) {
    // если пользователь был создан, то чистим очередь
    if (info.user_list[i].last_message_index != -1) {
      QueueClose(info.user_list[i].mqdes_client, info.user_list[i].queue_name);
    }
  }

  QueueClose(info.mqdes_service, SERVICE_QUEUE);
  QueueClose(info.mqdes_cl_message, CLIENT_MS_QUEUE);
  QueueClose(info.mqdes_send, SEND_QUEUE);
  QueueClose(info.mqdes_shutdown, SHUTDOWN_SERVER_QUEUE);

  printf(BLUE "LOG: server is closed\n" END_COLOR);

  return 0;
}