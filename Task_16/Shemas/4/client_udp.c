#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../color.h"

#define IP_ADDR "127.0.0.1"
#define SIZE_BUFF 100
#define UDP_PORT 9525

int main() {
  int cfd;
  struct sockaddr_in serv_addr;
  socklen_t sock_len = sizeof(serv_addr);
  char buff[SIZE_BUFF];
  int ip_addr;
  inet_pton(AF_INET, IP_ADDR, &ip_addr);
  memset(&serv_addr, 0, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(UDP_PORT);
  inet_pton(AF_INET, IP_ADDR, &serv_addr.sin_addr.s_addr);

  cfd = socket(AF_INET, SOCK_DGRAM, 0);

  if (cfd == -1) {
    printf("SOCKET ERROR: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  // if (connect(cfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
  //   printf("CONNECT ERROR: %s\n", strerror(errno));
  //   exit(EXIT_FAILURE);
  // }

  strncpy(buff, "time", 5);
  if (sendto(cfd, buff, SIZE_BUFF, 0, (struct sockaddr *)&serv_addr,
             sock_len) == -1) {
    printf(RED "SEND ERROR\n" END_COLOR);
  }

  // ждем сообщение от сервера
  if (recvfrom(cfd, buff, SIZE_BUFF, 0, (struct sockaddr *)&serv_addr,
               &sock_len) == -1) {
    printf(RED "RECV ERROR\n" END_COLOR);
  }

  printf("%s\n", buff);

  // отправляем сообщение о закрытии

  close(cfd);

  return 0;
}