#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "color.h"

#define IP_ADDR "127.0.0.2"
#define PORT 10000
#define SIZE_BUFF 100

void ErrorHandler(char *error_text) {
  printf(RED "%s: %s\n" END_COLOR, error_text, strerror(errno));
  exit(EXIT_FAILURE);
}

int main() {
  int sfd;
  struct sockaddr_in serv_addr, cl_addr;
  socklen_t sock_len = sizeof(serv_addr);
  char buff[SIZE_BUFF];
  int ip_addr;
  inet_pton(AF_INET, IP_ADDR, &ip_addr);
  memset(&serv_addr, 0, sizeof(serv_addr));
  memset(&cl_addr, 0, sizeof(cl_addr));

  sfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sfd == -1) {
    printf("SOCKET ERROR: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  serv_addr.sin_addr.s_addr = ip_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  if (bind(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
    printf("BIND ERROR: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  // ждем сообщение от клиента
  if (recvfrom(sfd, buff, SIZE_BUFF, 0, (struct sockaddr *)&cl_addr,
               &sock_len) == -1) {
    ErrorHandler("RECV FROM ERROR");
  }

  printf("%s\n", buff);
  buff[0] = 'A';
  if (sendto(sfd, buff, SIZE_BUFF, 0, (struct sockaddr *)&cl_addr, sock_len) ==
      -1) {
    ErrorHandler("SEND TO ERROR");
  }

  // ждем сообщение о получении
  if (recvfrom(sfd, buff, SIZE_BUFF, 0, (struct sockaddr *)&cl_addr,
               &sock_len) == -1) {
    ErrorHandler("RECV FROM ERROR");
  }
  printf("CLIENT MESSAGE: %s\n", buff);

  close(sfd);

  return 0;
}