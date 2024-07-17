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
#define MAX_THREAD 100
#define THREAD_IS_FREE -2

int cl_list_len = 100;
int shtdwn = 1;

typedef struct {
  pthread_t answer_thread;
  int client_fd;
  // условие, что сервер принял клиента
  pthread_cond_t client_accepted;
} Client;

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief Поток ожидает от пользователя ключевого слова time. В ответ отправляет
 * время и завершает с ним работу.
 * После завершения работы с клиентом функция меняет информацию в client_list.
 *
 * @return void*
 */
void *AnswerClient(void *arg) {
  printf(GREEN "START THREAD\n" END_COLOR);
  Client *client = (Client *)arg;

  char buff[SIZE_BUFF];
  // слушаем клиента
  while (shtdwn) {
    pthread_mutex_lock(&m1);

    pthread_cond_wait(&client->client_accepted, &m1);

    pthread_mutex_unlock(&m1);
    if (recv(client->client_fd, buff, SIZE_BUFF, 0) == -1 && shtdwn) {
      printf(RED "RECEIVE ERROR: %s\n" END_COLOR, strerror(errno));
    }
    printf(GREEN "GET MESSAGE FROM CLIENT %d\n" END_COLOR, client->client_fd);
    // клиент сможет отправить только "time", не обрабатываем информацию, а
    // сразу готовим ответ

    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // Формируем строку в формате YYYY.MM.DD | HH.MM.SS
    strftime(buff, sizeof(buff), "%Y.%m.%d | %H:%M:%S", timeinfo);

    if (send(client->client_fd, buff, SIZE_BUFF, 0) == -1 && shtdwn) {
      printf(RED "SEND ERROR: %s\n" END_COLOR, strerror(errno));
    }
    printf(GREEN "SEND MESSAGE TO CLIENT %d\n" END_COLOR, client->client_fd);

    // ждем сообщение о получении клиентом времени для закрытия сокета
    if (recv(client->client_fd, buff, SIZE_BUFF, 0) == -1 && shtdwn) {
      printf(RED "RECEIVE ERROR: %s\n" END_COLOR, strerror(errno));
    }

    pthread_mutex_lock(&m1);
    if (close(client->client_fd) == -1 && shtdwn) {
      printf(RED "CLOSE CLIENT SOCKET ERROR %d: %s\n" END_COLOR,
             client->client_fd, strerror(errno));
    }
    printf(GREEN "CLOSE CLIENT SOCKET %d\n" END_COLOR, client->client_fd);
    client->client_fd = THREAD_IS_FREE;
    pthread_mutex_unlock(&m1);
  }

  printf(GREEN "END THREAD\n" END_COLOR);

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
  int tmp_fd;

  Client client_list[MAX_THREAD];

  // создаем потоки
  for (int i = 0; i < MAX_THREAD; i++) {
    client_list[i].client_fd = THREAD_IS_FREE;
    pthread_cond_init(&client_list[i].client_accepted, NULL);
    pthread_create(&(client_list[i].answer_thread), NULL, AnswerClient,
                   &(client_list[i]));
  }

  while (shtdwn) {
    tmp_fd = accept(listener_sfd, (struct sockaddr *)listener_addr, &sock_len);

    // находим ближайший свободный поток
    pthread_mutex_lock(&m1);
    for (int i = 0; i < MAX_THREAD; i++) {

      if (client_list[i].client_fd == THREAD_IS_FREE) {
        client_list[i].client_fd = tmp_fd;
        printf(YELLOW "THREAD %d START WORK WTIH CLIENT %d\n" END_COLOR, i,
               client_list[i].client_fd);
        pthread_cond_signal(&client_list[i].client_accepted);
        break;
      }
    }
    pthread_mutex_unlock(&m1);
  }
  // убиваем все потоки закрываем открытые сокеты
  for (int i = 0; i < MAX_THREAD; i++) {
    pthread_cond_signal(&client_list[i].client_accepted);
    pthread_join(client_list[i].answer_thread, NULL);
  }
  printf(GREEN "LISTENER END\n" END_COLOR);
}

void sigint_handler(int signal) { shtdwn = 0; }

int main() {
  struct sigaction sg;
  sg.sa_flags = SA_RESTART;
  sg.sa_handler = sigint_handler;
  sigemptyset(&sg.sa_mask);

  sigaction(SIGINT, &sg, NULL);

  int listener_sfd, ip_addr, port;
  // список сокетов клиентов
  struct sockaddr_in listener_addr;

  int opt = 1;

  socklen_t sock_len = sizeof(listener_addr);
  inet_pton(AF_INET, IP_ADDR, &ip_addr);

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