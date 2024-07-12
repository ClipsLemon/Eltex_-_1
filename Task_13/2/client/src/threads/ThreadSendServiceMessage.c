#include "../../../color.h"
#include "../../client.h"
/*
поток создается при запуске клиента и принимает его никнейм, создает название
очереди и отправляет информацию о входе на сервер, по необходимо отправляет
сообщение о выходе
*/
void *ThreadSendServiceMessage() {
  ClientInf client_inf;
  mqd_t mqdes_server_message;

  Login(client_inf.username);

  // создаем название очереди
  client_inf.username[0] = CLIENT_CON;
  strncpy(client_inf.queue_name, client_inf.username, USERNAME_LEN + 1);
  client_inf.queue_name[0] = '/';
  // заходим на сервер и открываем очередь
  mq_send(mqdes_service, client_inf.username, SERVICE_MESSAGE_LEN, 1);
  // открываем очередь на чтение, сюда сервер рассылает сообщения истории
  sleep(1);
  mqdes_server_message =
      QueueConnect(client_inf.queue_name, DEFAULT_OFLGAS, DEFAULT_MODE,
                   MESSAGE_PACK_LEN, MAX_CL_MESSAGE);

  pthread_t send_message_thread;
  pthread_t recieve_message_thread;
  pthread_create(&send_message_thread, NULL, ThreadSendMessage, &client_inf);
  pthread_create(&recieve_message_thread, NULL, ThreadReceive,
                 &mqdes_server_message);

  // ждем завершения поток отправки сообщений
  pthread_join(send_message_thread, NULL);

  client_inf.username[0] = CLIENT_DISC;
  mq_send(mqdes_service, client_inf.username, SERVICE_MESSAGE_LEN, 1);
  // выходим из сервера и закрываем очередь
  QueueDisconnect(mqdes_server_message, client_inf.queue_name);
  return NULL;
}
