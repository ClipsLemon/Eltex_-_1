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

#define IP_ADDR "127.0.0.1"
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
  int fd;
  struct sockaddr_in address;
  socklen_t sock_len = sizeof(address);
  pthread_t exit_thread;

  char buff[SIZE_BUFF];
  memset(buff, 0, SIZE_BUFF);

  if (pthread_create(&exit_thread, NULL, ThreadExit, NULL) != 0) {
    ErrorHandler("THREAD CREATE ERROR");
  }
  fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
  if (fd == -1) {
    ErrorHandler("SOCKET ERROR");
  }

  memset(&address, 0, sizeof(address));

  inet_pton(AF_INET, IP_ADDR, &address.sin_addr.s_addr);
  address.sin_family = AF_INET;
  address.sin_port = htons(PORT);



  while (shtdwn) {
    recvfrom(fd, buff, SIZE_BUFF, 0, (struct sockaddr *)&address, &sock_len);
    for (int i = 0; i < SIZE_BUFF; i++) {
      printf("%c", buff[i]);
    }
    printf("\n");
  }

  close(fd);

  return 0;
}