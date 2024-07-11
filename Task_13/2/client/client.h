#ifndef CLIENT_H
#define CLIENT_H

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

#define DEFAULT_OFLGAS (O_RDONLY)
#define DEFAULT_MODE (S_IWUSR | S_IRUSR)

typedef struct {
  int last_message_index;
  char username[USERNAME_LEN];
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

extern mqd_t mqdes_cl_message;
extern mqd_t mqdes_service;
extern mqd_t mqdes_send;
extern int history_index;
extern Message chat_history[HISTORY_LEN];

/*
Функция создает очередь и обрабатывает ошибки связанные с ее созданием*/
mqd_t QueueConnect(char *queue_name, int oflag, mode_t mode, int message_len,
                   int number_of_messages);

/*
Функция закрывает очередь и обрабатыват ошибки связанные с ее закрытием
*/
void QueueDisconnect(mqd_t queue_id, char *queue_name);

void GetName(char *string);
void ClearString(char *string, int len);
void RemoveNewLineSymbol(char *string);
void CreateMessage(char *message, char *username);
void ParseServerMessage(char *message);
// ----------------- ПОТОКИ -------------------
// void *ThreadServiceReceive(void *mqdes_service);
void *ThreadReceive(void *arg);

#endif // CLIENT_H