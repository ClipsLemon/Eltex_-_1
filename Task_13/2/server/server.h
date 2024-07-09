#ifndef SERVER_H
#define SERVER_H

#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// с учетом \0
#define MESSAGE_SIZE 1024
// максимальное количество сообщений, пока что статическое
#define MAX_MESSAGE 50
// 2014.01.01 12:12 с учетом \0
#define DATE_TIME_SIZE 17
// максимальный размер истории
#define HISTORY_LEN 150
// максимальная длина имени пользователя с учетом \n
#define USERNAME_LEN 20
// пока что статически
#define USERS_MAX 10

// клиент вошел на сервер
#define CLIENT_EXIT 'e'
// клиент вышел с сервера
#define CLIENT_INPUT 'i'

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

#define DEFAULT_OFLGAS (O_CREAT | O_RDWR)
#define DEFAULT_MODE (S_IWUSR | S_IRUSR)

typedef struct {
  int last_message_index;
  char username[USERNAME_LEN];
  mqd_t mqdes_client;
} User;

typedef struct {
  char username[USERNAME_LEN];
  char message[MESSAGE_SIZE];
} Message;

typedef struct {
  char message_type;
  char username[USERNAME_LEN];
} ServiceMessage;

/*
Функция создает очередь и обрабатывает ошибки связанные с ее созданием*/
mqd_t QueueOpen(char *queue_name, int oflag, mode_t mode, struct mq_attr *attr);

/*
Функция закрывает очередь и обрабатыват ошибки связанные с ее закрытием
*/
void QueueClose(mqd_t queue_id, char *queue_name);

#endif // SERVER_H