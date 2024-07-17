#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "../color.h"

#define LISTEN_BACKLOG 4
#define IP_ADDR "127.0.0.1"
#define SIZE_BUFF 100
#define MAX_PORT 65535
#define SERV_PORT 9524
#define MAX_THREAD 10

int cl_list_len = 100;
int shtdwn = 1;

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;

void *ThreadExit() {
  printf(GREEN "EXIT THREAD HAS BEEN CREATED\n" END_COLOR);
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  char ext[6];
  char buff[SIZE_BUFF];

  struct sockaddr_in tmp;
  inet_pton(AF_INET, IP_ADDR, &tmp.sin_addr.s_addr);
  tmp.sin_family = AF_INET;
  tmp.sin_port = htons(SERV_PORT);

  memset(ext, 0, 6);

  while (strncmp(ext, "exit", 5) != 0) {
    fgets(ext, 5, stdin);
  }
  // если пользователь сервера вводит exit, то коннектимся к самому себе что бы
  // сбить accept в ListenClient и ставит shtdwn в 0
  shtdwn = 0;

  if (connect(sock, (struct sockaddr *)&tmp, sizeof(tmp)) == -1) {
    printf(RED "CONNECT ERROR: %s" END_COLOR, strerror(errno));
  }
  strncpy(buff, "time", 5);
  send(sock, buff, SIZE_BUFF, 0);
  strncpy(buff, "ACCEPT", 7);
  send(sock, buff, SIZE_BUFF, 0);

  printf(GREEN "EXIT THREAD HAS BEEN CLOSED\n" END_COLOR);
  return NULL;
}

/**
 * @brief Поток ожидает от пользователя ключевого слова time. В ответ отправляет
 * время и завершает с ним работу.
 * После завершения работы с клиентом функция меняет информацию в client_list.
 *
 * @return void*
 */
void *AnswerClient(void *arg) {
  int *client_fd = (int *)arg;
  char buff[SIZE_BUFF];
  // слушаем клиента
  if (recv(*client_fd, buff, SIZE_BUFF, 0) == -1) {
    printf(RED "RECEIVE ERROR: %s\n" END_COLOR, strerror(errno));
  }
  printf(GREEN "GET MESSAGE FROM CLIENT %d\n" END_COLOR, *client_fd);
  // клиент сможет отправить только "time", не обрабатываем информацию, а сразу
  // готовим ответ

  time_t rawtime;
  struct tm *timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);

  // Буфер для хранения строки с датой и временем

  // Формируем строку в формате YYYY.MM.DD | HH.MM.SS
  strftime(buff, sizeof(buff), "%Y.%m.%d | %H:%M:%S", timeinfo);

  if (send(*client_fd, buff, SIZE_BUFF, 0) == -1) {
    printf(RED "SEND ERROR: %s\n" END_COLOR, strerror(errno));
  }
  printf(GREEN "SEND MESSAGE TO CLIENT %d\n" END_COLOR, *client_fd);

  // ждем сообщение о получении клиентом времени для закрытия сокета
  if (recv(*client_fd, buff, SIZE_BUFF, 0) == -1) {
    printf(RED "RECEIVE ERROR: %s\n" END_COLOR, strerror(errno));
  }

  pthread_mutex_lock(&m1);
  if (close(*client_fd) == -1) {
    printf(RED "CLOSE CLIENT SOCKET ERROR %d: %s\n" END_COLOR, *client_fd,
           strerror(errno));
  }
  pthread_mutex_unlock(&m1);
  printf(GREEN "CLOSE CLIENT SOCKET %d\n" END_COLOR, *client_fd);

  return NULL;
}

/**
 * @brief прослушивает клиентов на подключение. В случае подключения создает
 * поток в котором пользователь запрашивает время у сервера
 *
 * @return * void
 */
void ListenClient(int listener_sfd, struct sockaddr_in *listener_addr,
                  socklen_t sock_len) {
  printf(GREEN "LISTENER START\n" END_COLOR);
  int *client_list;
  int curr_client = 0;
  pthread_t *thread_list;
  pthread_t thread_exit;
  pthread_create(&thread_exit, NULL, ThreadExit, NULL);

  client_list = (int *)calloc(cl_list_len, sizeof(int) * cl_list_len);
  thread_list =
      (pthread_t *)calloc(cl_list_len, sizeof(pthread_t) * cl_list_len);

  int tmp_fd;
  while (shtdwn) {
    tmp_fd = accept(listener_sfd, (struct sockaddr *)listener_addr, &sock_len);
    pthread_mutex_lock(&m1);
    client_list[curr_client] = tmp_fd;
    if (curr_client == cl_list_len - 1) {
      cl_list_len += 100;
      client_list = (int *)realloc(client_list, sizeof(int) * cl_list_len);
      thread_list =
          (pthread_t *)realloc(thread_list, sizeof(pthread_t) * cl_list_len);
    }
    pthread_mutex_unlock(&m1);
    if (pthread_create(&thread_list[curr_client], NULL, AnswerClient,
                       &client_list[curr_client]) != 0) {
      printf(RED "THREAD CREATE ERROR: %s" END_COLOR, strerror(errno));
    }
    curr_client++;
  }
  // ждем закрытия всех потоков, дорабатываем с клиентами
  for (int i = 0; i < cl_list_len; i++) {
    pthread_join(thread_list[i], NULL);
  }
  free(thread_list);
  free(client_list);
  printf(GREEN "LISTENER END\n" END_COLOR);
}

int main() {

  int listener_sfd, ip_addr, port;
  // список сокетов клиентов
  inet_pton(AF_INET, IP_ADDR, &ip_addr);

  struct sockaddr_in listener_addr;

  int opt = 1;

  socklen_t sock_len = sizeof(listener_addr);

  listener_sfd = socket(AF_INET, SOCK_STREAM, 0);

  // Настраиваем сокет для повторного использования адреса и порта
  if (setsockopt(listener_sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    perror("setsockopt");
    close(listener_sfd);
    exit(EXIT_FAILURE);
  }

  if (listener_sfd == -1) {
    printf("SOCKET ERROR: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  memset(&listener_addr, 0, sizeof(listener_addr));

  listener_addr.sin_family = AF_INET;
  listener_addr.sin_port = htons(SERV_PORT);
  listener_addr.sin_addr.s_addr = ip_addr;

  while (1) {
    if (bind(listener_sfd, (struct sockaddr *)&listener_addr,
             sizeof(listener_addr)) == -1) {
      printf(RED "BIND ERROR: %s\n" END_COLOR, strerror(errno));
      // возможно была ранее сервер некорректно завершил работу, ребинд
      close(listener_sfd);
      sleep(1);
    } else {
      break;
    }
  }

  if (listen(listener_sfd, LISTEN_BACKLOG) == -1) {
    printf("LISTEN ERROR: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  // начинаем слушать клиентов
  ListenClient(listener_sfd, &listener_addr, sock_len);

  return 0;
}