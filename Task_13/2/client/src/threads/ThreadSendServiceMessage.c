#include "../../../color.h"
#include "../../client.h"
/*
поток создается при запуске клиента и принимает его никнейм, создает название
очереди и отправляет информацию о входе на сервер, по необходимо отправляет
сообщение о выходе
*/

void *ThreadSendServiceMessage(void *arg) {
  Controller *info = (Controller *)arg;

  Login(info);

  // создаем название очереди
  info->client_inf.username[0] = CLIENT_CON;
  strncpy(info->client_inf.queue_name, info->client_inf.username,
          USERNAME_LEN + 1);
  info->client_inf.queue_name[0] = '/';
  // заходим на сервер и открываем очередь
  mq_send(info->mqdes_service, info->client_inf.username, SERVICE_MESSAGE_LEN,
          1);
  // открываем очередь на чтение, сюда сервер рассылает сообщения истории
  sleep(1);
  info->mqdes_server_msg =
      QueueConnect(info->client_inf.queue_name, DEFAULT_OFLGAS, DEFAULT_MODE,
                   MESSAGE_PACK_LEN, MAX_CL_MESSAGE, info->log_file);

  pthread_t send_message_thread;
  pthread_t recieve_message_thread;
  pthread_create(&send_message_thread, NULL, ThreadSendMessage, (void *)info);
  pthread_create(&recieve_message_thread, NULL, ThreadReceive, (void *)info);

  // ждем завершения поток отправки сообщений
  pthread_join(send_message_thread, NULL);

  info->client_inf.username[0] = CLIENT_DISC;
  mq_send(info->mqdes_service, info->client_inf.username, SERVICE_MESSAGE_LEN,
          1);
  // выходим из сервера и закрываем очередь
  QueueDisconnect(info->mqdes_server_msg, info->client_inf.queue_name);
  return NULL;
}
