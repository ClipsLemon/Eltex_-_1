#ifndef CLIENT_H
#define CLIENT_H

#include <errno.h>
// #include <ncurses.h>
#include <curses.h>
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
#define SEM_SEND_MESS "/send_message"

#define DEFAULT_OFLGAS (O_RDWR)
#define DEFAULT_MODE (S_IWUSR | S_IRUSR)

typedef struct {
  char username[USERNAME_LEN];
  char datetime[DATE_TIME_SIZE];
  char message[CL_MESSAGE_LEN];
} Message;

typedef struct {
  char user_list[USERS_MAX][USERNAME_LEN];
  int history_index;
  int readed_index;
  Message chat_history[HISTORY_LEN];
} Chat;

typedef struct {
  // индекс прочитанной истории чата
  FILE *log_file;
  char username[USERNAME_LEN];
  sem_t *send_message;
  WINDOW *win_chat_field;
  WINDOW *win_text_field;
  WINDOW *win_users_field;
  Chat *chat;
} Controller;

extern pthread_mutex_t m1;

/**
 * @brief Функция берет имя у пользователя при логине
 * ы
 * @param string - указатель на строку в которую запишется имя
 */
void GetName(char *string);

/**
 * @brief Удаляет символ переноса строки
 *
 * @param string - указатель на строку где удаляем символ
 */
void RemoveNewLineSymbol(char *string);

/**
 * @brief Функция создает
 *
 * @param message
 * @param username
 */
void CreateMessage(Controller *info, char *message);

/**
 * @brief Функция выводит список пользователей
 *
 * @param info - адрес на контроллер
 */
void PrintUsersList(Controller *info);

/**
 * @brief Функция логинит клиента через окно аутентификации
 *
 * @param info - адрес на контроллер
 */
void Login(Controller *info);

/**
 * @brief Функция вывода чат сообщений
 *
 * @param info - адрес на контроллер
 */
void PrintChat(Controller *info);

// ----------------- ПОТОКИ -------------------

/**
 * @brief Поток отправляющий сообщения на сервер
 *
 * @param arg
 * @return void*
 */
void *ThreadSendMessage(void *arg);
void *ThreadPrintChat(void *arg);
void *ThreadFindUsers(void *arg);

void PrintTextField(Controller *info);

#endif // CLIENT_H