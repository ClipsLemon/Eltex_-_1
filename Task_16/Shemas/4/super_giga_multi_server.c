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
#define TCP_PORT 9524
#define UDP_PORT 9525

#define MAX_EVENTS 1000

void GetTime(char *time_string) {
  time_t rawtime;
  struct tm *timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);

  // Формируем строку в формате YYYY.MM.DD | HH:MM:SS
  strftime(time_string, SIZE_BUFF, "%Y.%m.%d | %H:%M:%S", timeinfo);
}

/**
 * @brief Создает сокет и структуру под него
 *
 * @param socket_fd
 */
void CreateSocket(int *socket_fd, struct sockaddr_in *socket_addr, int port,
                  int type) {
  // Создание серверного сокета
  if ((*socket_fd = socket(AF_INET, type, 0)) == 0) {
    printf(RED "SOCKET ERRROR: %s\n" END_COLOR, strerror(errno));

    exit(EXIT_FAILURE);
  }

  // Установка параметров адреса сервера
  socket_addr->sin_family = AF_INET;
  inet_pton(AF_INET, IP_ADDR, &(socket_addr->sin_addr.s_addr));
  socket_addr->sin_port = htons(port);
}

void AddEvent(int epollfd, int fd, int events) {
  struct epoll_event ev;
  ev.events = events;
  ev.data.fd = fd;
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
    printf(RED "EPOLL CTL ERROR: %s\n" END_COLOR, strerror(errno));
  }
}

int main() {
  int tcp_fd, udp_fd, new_socket, epoll_fd, event_count, i, valread;
  struct sockaddr_in tcp_address, udp_address;
  struct epoll_event event, events[MAX_EVENTS];
  char buffer[SIZE_BUFF];
  time_t rawtime;
  struct tm *timeinfo;

  socklen_t addr_len;
  addr_len = sizeof(tcp_address);

  // ------------ TCP INITIALIZATION -------------
  CreateSocket(&tcp_fd, &tcp_address, TCP_PORT, SOCK_STREAM);

  // Привязка сокета к адресу
  if (bind(tcp_fd, (struct sockaddr *)&tcp_address, sizeof(tcp_address)) < 0) {
    printf(RED "BIND ERROR: %s\n" END_COLOR, strerror(errno));

    close(tcp_fd);
    exit(EXIT_FAILURE);
  }

  // Прослушивание входящих подключений
  if (listen(tcp_fd, 3) < 0) {
    printf(RED "LISTEN ERROR: %s\n" END_COLOR, strerror(errno));

    close(tcp_fd);
    exit(EXIT_FAILURE);
  }
  // --------------------------------------------

  // ------------ UDP INITIALIZATION -------------
  CreateSocket(&udp_fd, &udp_address, UDP_PORT, SOCK_DGRAM);

  // Привязка сокета к адресу
  if (bind(udp_fd, (struct sockaddr *)&udp_address, sizeof(udp_address)) < 0) {
    printf(RED "BIND ERROR: %s\n" END_COLOR, strerror(errno));

    close(udp_fd);
    exit(EXIT_FAILURE);
  }
  // --------------------------------------------

  // Создание epoll
  if ((epoll_fd = epoll_create1(0)) == -1) {
    printf(RED "EPOLL CREATE ERROR: %s\n" END_COLOR, strerror(errno));

    close(tcp_fd);
    exit(EXIT_FAILURE);
  }

  AddEvent(epoll_fd, tcp_fd, EPOLLIN);
  AddEvent(epoll_fd, udp_fd, EPOLLIN);

  printf("Listening on port %d \n", TCP_PORT);
  printf("Listening on port %d \n", UDP_PORT);

  while (1) {
    // Ожидание событий
    event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    if (event_count == -1) {
      printf(RED "EPOLL_WAIT ERROR: %s\n" END_COLOR, strerror(errno));

      close(tcp_fd);
      close(epoll_fd);
      exit(EXIT_FAILURE);
    }

    for (i = 0; i < event_count; i++) {
      // подключение по TCP
      if (events[i].data.fd == tcp_fd) {
        // Обработка новых подключений
        new_socket = accept(tcp_fd, (struct sockaddr *)&tcp_address, &addr_len);
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
      } else if (events[i].data.fd == udp_fd) {
        // сообщение по UDP
        // ждем time от клиента
        recvfrom(udp_fd, buffer, SIZE_BUFF, 0, (struct sockaddr *)&udp_address,
                 &addr_len);
        printf(YELLOW "GET MESSAGE ON UDP SOCKET\n" END_COLOR);
        GetTime(buffer);

        if (sendto(events[i].data.fd, buffer, SIZE_BUFF, 0,
                   (struct sockaddr *)&udp_address, addr_len) == -1) {
          printf(RED "SEND ERROR: %s\n" END_COLOR, strerror(errno));
        }
        printf(YELLOW "SEND MESSAGE TO UDP SOCKET\n" END_COLOR);
      } else {
        // Обработка отключений по TCP
        valread = read(events[i].data.fd, buffer, SIZE_BUFF);
        if (valread == -1) {
          perror("read");
          close(events[i].data.fd);
        } else if (valread == 0) {
          // Отключение клиента
          printf(YELLOW "DISCONNECTED FD %d\n" END_COLOR, events[i].data.fd);
          close(events[i].data.fd);
        } else {
          // Обработка сообщения клиента TCP
          GetTime(buffer);

          if (send(events[i].data.fd, buffer, SIZE_BUFF, 0) == -1) {
            printf(RED "SEND ERROR: %s\n" END_COLOR, strerror(errno));
          }
        }
      }
    }
  }

  close(tcp_fd);
  close(epoll_fd);

  return 0;
}