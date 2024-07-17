#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define LISTEN_BACKLOG 4
#define IP_ADDR "127.0.0.1"
#define SIZE_BUFF 100
#define MAX_PORT 65535
#define SERV_PORT 9524

int main() {
  int server_fd;
  struct sockaddr_in serv_addr;
  socklen_t serv_sock_len = sizeof(serv_addr);
  int ip_addr;
  inet_pton(AF_INET, IP_ADDR, &ip_addr);

  char buff[SIZE_BUFF];
  memset(buff, 0, SIZE_BUFF);

  server_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (server_fd == -1) {
    printf("SOCKET ERROR: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = ip_addr;
  serv_addr.sin_port = htons(SERV_PORT);

  if (connect(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) ==
      -1) {
    printf("CONNECT ERROR: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  strncpy(buff, "time", 5);
  send(server_fd, buff, SIZE_BUFF, 0);

  // ожидаем сообщения от сервера
  recv(server_fd, buff, SIZE_BUFF, 0);
  printf("%s\n", buff);

  return 0;
}