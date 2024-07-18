#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "color.h"

#define BROADCAST_IP "255.255.255.255"
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

int main() {
  int server_fd;
  struct sockaddr_in broadcast_addr;
  socklen_t sock_len = sizeof(broadcast_addr);
  char buff[SIZE_BUFF];
  int flag = 1;

  pthread_t exit_thread;

  if (pthread_create(&exit_thread, NULL, ThreadExit, NULL) != 0) {
    ErrorHandler("THREAD CREATE ERROR");
  }

  memset(&broadcast_addr, 0, sock_len);

  server_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (server_fd == -1) {
    ErrorHandler("SOCKET ERROR");
  }
  setsockopt(server_fd, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag));

  inet_pton(AF_INET, BROADCAST_IP, &broadcast_addr.sin_addr.s_addr);
  broadcast_addr.sin_family = AF_INET;
  broadcast_addr.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *)&broadcast_addr, sock_len) == -1) {
    ErrorHandler("BIND ERROR");
  }

  while (shtdwn) {
    if (recvfrom(server_fd, buff, SIZE_BUFF, 0,
                 (struct sockaddr *)&broadcast_addr, &sock_len) == -1) {
      ErrorHandler("RECEFROM ERROR");
    }
    printf(GREEN "SERVER MESSAGE: %s\n" END_COLOR, buff);
    memset(buff, 0, SIZE_BUFF);
    sleep(1);
  }

  return 0;
}