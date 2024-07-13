#ifndef SERVER_H
#define SERVER_H

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/mman.h>
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

#define SHM_NAME "messenger"
#define SEM_NAME "/shutdown_semaphore"

#define DEFAULT_OFLGAS (O_CREAT | O_RDWR)
#define DEFAULT_MODE (S_IWUSR | S_IRUSR)
#define SEM_SEND_MESS "/send_message"

typedef struct {
  char username[USERNAME_LEN];
  char datetime[DATE_TIME_SIZE];
  char message[CL_MESSAGE_LEN];
} Message;

typedef struct {
  char user_list[USERS_MAX][USERNAME_LEN];

  int history_index;
  int readed_index;
  // shared memory object
  Message chat_history[HISTORY_LEN];
} Controller;

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
void *ThreadExit();
/**
 * @brief Поток обрабатывает очередь выключения, для завершения потоков
 *
 * @return void*
 */
void *ThreadShutdown(void *arg);

#endif // SERVER_H