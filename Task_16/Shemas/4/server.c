#include <arpa/inet.h>
#include <errno.h>
#include <mqueue.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "../color.h"

#define LISTEN_BACKLOG 4
#define IP_ADDR "127.0.0.1"
#define SIZE_BUFF 100
#define SERV_PORT 9524
#define MAX_EVENTS 1000

int shtdwn = 1;
mqd_t mqdes_client_queue;

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;

int main() {
  int server_fd, new_socket, epoll_fd, event_count, i, valread;
  struct sockaddr_in address;
  struct epoll_event event, events[MAX_EVENTS];
  char buffer[SIZE_BUFF];
  time_t rawtime;
  struct tm *timeinfo;

  socklen_t addr_len;
  addr_len = sizeof(address);
  // Создание серверного сокета
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    printf(RED "SOCKET ERRROR: %s\n" END_COLOR, strerror(errno));

    exit(EXIT_FAILURE);
  }

  // Установка параметров адреса сервера
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(SERV_PORT);

  // Привязка сокета к адресу
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    printf(RED "BIND ERROR: %s\n" END_COLOR, strerror(errno));

    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // Прослушивание входящих подключений
  if (listen(server_fd, 3) < 0) {
    printf(RED "LISTEN ERROR: %s\n" END_COLOR, strerror(errno));

    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("Listening on port %d \n", SERV_PORT);

  // Создание epoll инстанции
  if ((epoll_fd = epoll_create1(0)) == -1) {
    printf(RED "EPOLL CREATE ERROR: %s\n" END_COLOR, strerror(errno));

    close(server_fd);
    exit(EXIT_FAILURE);
  }

  event.events = EPOLLIN;
  event.data.fd = server_fd;

  // Регистрация серверного сокета в epoll
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
    printf(RED "EPOLL_CTL SERVER ERROR: %s\n" END_COLOR, strerror(errno));

    close(server_fd);
    close(epoll_fd);
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Ожидание событий
    event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    if (event_count == -1) {
      printf(RED "EPOLL_WAIT ERROR: %s\n" END_COLOR, strerror(errno));

      close(server_fd);
      close(epoll_fd);
      exit(EXIT_FAILURE);
    }

    for (i = 0; i < event_count; i++) {
      if (events[i].data.fd == server_fd) {
        // Обработка новых подключений
        new_socket = accept(server_fd, (struct sockaddr *)&address, &addr_len);
        if (new_socket == -1) {
          printf(RED "ACCEPT ERROR: %s\n" END_COLOR, strerror(errno));
        } else {
          printf(YELLOW "CONNECTED FD %d\n" END_COLOR, new_socket);
          event.events = EPOLLIN | EPOLLET;
          event.data.fd = new_socket;
          if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &event) == -1) {
            printf(RED "EPOLL_CTL ERROR: %s\n" END_COLOR, strerror(errno));
            close(new_socket);
          }
        }
      } else {
        // Обработка сообщений от клиентов
        valread = read(events[i].data.fd, buffer, SIZE_BUFF);
        if (valread == -1) {
          perror("read");
          close(events[i].data.fd);
        } else if (valread == 0) {
          // Отключение клиента
          printf(YELLOW "DISCONNECTED FD %d\n" END_COLOR, events[i].data.fd);
          close(events[i].data.fd);
        } else {
          // Обработка сообщения клиента
          time(&rawtime);
          timeinfo = localtime(&rawtime);

          // Формируем строку в формате YYYY.MM.DD | HH:MM:SS
          strftime(buffer, sizeof(buffer), "%Y.%m.%d | %H:%M:%S", timeinfo);

          if (send(events[i].data.fd, buffer, SIZE_BUFF, 0) == -1) {
            printf(RED "SEND ERROR: %s\n" END_COLOR, strerror(errno));
          }
        }
      }
    }
  }

  close(server_fd);
  close(epoll_fd);

  return 0;
}