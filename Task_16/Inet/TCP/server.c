#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define LISTEN_BACKLOG 1
#define IP_ADDR "127.0.0.1"
#define PORT 9234
#define SIZE_BUFF 20

int main() {
  int sfd, cfd;
  struct sockaddr_in server_addr, client_addr;
  socklen_t serv_sock_len = sizeof(server_addr);

  int ip_addr;
  inet_pton(AF_INET, IP_ADDR, &ip_addr);
  char buff[SIZE_BUFF];

  sfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sfd == -1) {
    printf("SOCKET ERROR: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  memset(buff, 0, SIZE_BUFF);
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = ip_addr;

  if (bind(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
    printf("BIND ERROR: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  if (listen(sfd, LISTEN_BACKLOG) == -1) {
    printf("LISTEN ERROR: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  cfd = accept(sfd, (struct sockaddr *)&server_addr, &serv_sock_len);

  // ожидаем сообщения от клиента
  recv(cfd, buff, SIZE_BUFF, 0);

  printf("%s\n", buff);

  // отправляем ответку
  strncpy(buff, "Hi!", 4);
  send(cfd, buff, SIZE_BUFF, 0);

  // ждем выхода клиента
  recv(cfd, buff, SIZE_BUFF, 0);
  close(sfd);
  close(cfd);

  return 0;
}
