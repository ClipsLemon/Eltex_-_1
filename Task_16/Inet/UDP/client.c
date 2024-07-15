#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define IP_ADDR "127.0.0.1"
#define PORT 9234
#define SIZE_BUFF 20

int main() {
  int cfd;
  struct sockaddr_in serv_addr;
  socklen_t sock_len = sizeof(serv_addr);
  char buff[SIZE_BUFF];
  int ip_addr;
  inet_pton(AF_INET, IP_ADDR, &ip_addr);
  memset(&serv_addr, 0, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  serv_addr.sin_addr.s_addr = ip_addr;

  cfd = socket(AF_INET, SOCK_DGRAM, 0);

  if (cfd == -1) {
    printf("SOCKET ERROR: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  if (connect(cfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
    printf("CONNECT ERROR: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  strncpy(buff, "Hello server!", 14);
  send(cfd, buff, SIZE_BUFF, 0);

  // ждем сообщение от сервера
  recv(cfd, buff, SIZE_BUFF, 0);

  printf("%s\n", buff);

  // отправляем сообщение о закрытии
  strncpy(buff, "Bye server!", 12);
  send(cfd, buff, sizeof(buff), 0);

  sleep(1);

  close(cfd);

  return 0;
}