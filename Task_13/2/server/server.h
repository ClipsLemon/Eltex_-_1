#ifndef SERVER_H
#define SERVER_H

#include <errno.h>
#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

// максимальная длинная сообщения пользователя
#define CL_MESSAGE_LEN 256
// максимальное количество сообщений, пока что статическое
#define MAX_CL_MESSAGE 50

// длина служебного сообщения : вход или выход + имя пользвателя
#define SERVICE_MESSAGE_LEN 21
// максимальное количество служебных сообщений
#define MAX_SERVICE_MESSAGE 10

// например "2014.01.01 12:12" с учетом \0
#define DATE_TIME_SIZE 17

// максимальный размер истории
#define HISTORY_LEN 150

// максимальная длина имени пользователя с учетом \n
#define USERNAME_LEN 20
// пока что статически колиетсво пользователей
#define USERS_MAX 10

// клиент вошел на сервер
#define CLIENT_DISC 'd'
// клиент вышел с сервера
#define CLIENT_CON 'c'

// совокупная длина пакета сообщения
#define MESSAGE_PACK_LEN (USERNAME_LEN + DATE_TIME_SIZE + CL_MESSAGE_LEN)

// служебная очередь, сюда клиенты отправляют сообщения о входе и выходе
#define SERVICE_QUEUE "/service_queue"
// очередь сообщений, сюда клиенты присылают свои сообщения
#define CLIENT_MS_QUEUE "/messages_queue"

#define CLIENT_LIST_QUEUE "/cl_lst_queue"
/*
 * очередь для сервера. В эту очередь, после обработки сообщения от клиента из
 * CLIENT_MS_QUEUE поступает сообщение, говорящие о том, какое сообщение надо
 * отправить всем клиентам
 */
#define SEND_QUEUE "/send_queue"

/*
Служебная очередь, в которую будет посылаться сигнал на выключение сервера.
По большей части нужна для работы сервера в режиме ожидания без затраты лишних
ресурсов.
*/
#define SHUTDOWN_SERVER_QUEUE "/shutdown"

#define DEFAULT_OFLGAS (O_CREAT | O_RDWR)
#define DEFAULT_MODE (S_IWUSR | S_IRUSR)

typedef struct {
  // -1 - значит пользователя не существует
  int last_message_index;
  char username[USERNAME_LEN];
  // + на / в начале названия
  char queue_name[USERNAME_LEN + 1];
  mqd_t mqdes_client;

} User;

typedef struct {
  char username[USERNAME_LEN];
  char datetime[DATE_TIME_SIZE];
  char message[CL_MESSAGE_LEN];
} Message;

typedef struct {
  char message_type;
  char username[USERNAME_LEN];
} ServiceMessage;

typedef struct {
  User user_list[USERS_MAX];

  mqd_t mqdes_service;
  mqd_t mqdes_cl_message;
  // сюда отправляется сообщение о необходимости обновить у всех клиентов
  // историю чата
  mqd_t mqdes_send;
  mqd_t mqdes_shutdown;
  int history_index;

  Message chat_history[HISTORY_LEN];
} Controller;

extern pthread_mutex_t m1;

// ------------- ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ ----------------

// ----------------------------------------------------

/**
 * @brief Функция удаляет клиента, когда пользователь присылает сообщение о
 * выходе. После чего функция отправляет сигнал на рассылку изменений по
 * клиентам
 *
 * @param clientname - имя клиента
 */
void DeleteClient(char *clientname, Controller *info);
/**
 * @brief Функция добавляет клиента, когда пользователь присылает сообщение о
 * входе. После чего функция отправляет сигнал на рассылку изменений по клиентам
 *
 * @param clientname - имя клиента
 */
int AddClient(char *clientname, Controller *info);

/**
 * @brief Функция подключает к очереди сообщений и обрабатывает ошибки связанные
 * с этим
 *
 * @param queue_name - название очереди
 * @param oflag
 * @param mode
 * @param message_len - длина сообщения в очереди
 * @param number_of_messages - количество сообщений в очереди
 * @return mqd_t
 */
mqd_t QueueOpen(char *queue_name, int oflag, mode_t mode, int message_len,
                int number_of_messages);

/**
 * @brief Функция закрывает очередь и обрабатыват ошибки связанные с ее
 * закрытием
 *
 * @param queue_id - id очереди
 * @param queue_name - название очереди
 */
void QueueClose(mqd_t queue_id, char *queue_name);

// ----------------- ПОТОКИ -------------------
/**
 * @brief Поток обрабатывает сервисные сообщения от клиентов
 *
 * @param mqdes_service
 * @return void*
 */
void *ThreadServiceReceive(void *mqdes_service);
/**
 * @brief Поток обрабатывает очередь сообщений от клиентов
 *
 * @return void*
 */
void *ThreadClMessage(void *arg);
/**
 * @brief Поток отправляет сообщения на клиентов
 *
 * @return void*
 */
void *ThreadSend(void *arg);
/**
 * @brief Поток обрабатывает очередь выключения, для завершения потоков
 *
 * @return void*
 */
void *ThreadShutdown(void *arg);

#endif  // SERVER_H