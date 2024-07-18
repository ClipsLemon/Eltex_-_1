#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "color.h"

#define MULTICAST_IP "224.0.0.1"
#define PORT 9234
#define SIZE_BUFF 100

int shtdwn = 1;

void ErrorHandler(char *error_text) {
  printf(RED "%s: %s\n" END_COLOR, error_text, strerror(errno));
  exit(EXIT_FAILURE);
}

void *ThreadExit(void *arg) {
  printf(GREEN "EXIT THREAD HAS BEEN CREATED\n" END_COLOR);
  char ext[6];

  memset(ext, 0, 6);

  while (strncmp(ext, "exit", 5) != 0) {
    fgets(ext, 5, stdin);
  }

  shtdwn = 0;

  printf(GREEN "EXIT THREAD HAS BEEN CLOSED\n" END_COLOR);

  return NULL;
}

void GetTime(char *time_string) {
  time_t rawtime;
  struct tm *timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);

  // Формируем строку в формате YYYY.MM.DD | HH:MM:SS
  strftime(time_string, SIZE_BUFF, "%Y.%m.%d | %H:%M:%S", timeinfo);
}

int main() {
  int server_fd;
  struct sockaddr_in multicast_addr;
  pthread_t exit_thread;
  socklen_t sock_len = sizeof(multicast_addr);
  char buff[SIZE_BUFF];
  int flag = 1;

  memset(&multicast_addr, 0, sock_len);
  inet_pton(AF_INET, MULTICAST_IP, &multicast_addr.sin_addr.s_addr);
  multicast_addr.sin_family = AF_INET;
  multicast_addr.sin_port = htons(PORT);

  struct ip_mreq mreqn;
  inet_pton(AF_INET, MULTICAST_IP, &mreqn.imr_multiaddr.s_addr);
  mreqn.imr_interface.s_addr = htonl(INADDR_ANY);

  if (pthread_create(&exit_thread, NULL, ThreadExit, NULL) != 0) {
    ErrorHandler("THREAD CREATE ERROR");
  }

  server_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (server_fd == -1) {
    ErrorHandler("SOCKET ERROR");
  }

  if (bind(server_fd, (struct sockaddr *)&multicast_addr, sock_len) == -1) {
    ErrorHandler("BIND ERROR");
  }

  setsockopt(server_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreqn, sizeof(mreqn));

  while (shtdwn) {
    if (recvfrom(server_fd, buff, SIZE_BUFF, 0,
                 (struct sockaddr *)&multicast_addr, &sock_len) == -1) {
      ErrorHandler("RECVFROM ERROR");
    }
    printf(GREEN "MESSAGE FROM SERVER: %s\n" END_COLOR, buff);
    memset(buff, 0, SIZE_BUFF);
    sleep(1);
  }

  close(server_fd);

  return 0;
}
