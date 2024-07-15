#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define ADDR "/local_server"

#define SIZE_BUFF 20

int main() {
  int cfd;
  struct sockaddr_un serv_addr;
  int len_peer_addr;

  char buff[SIZE_BUFF];
  memset(buff, 0, SIZE_BUFF);

  cfd = socket(AF_LOCAL, SOCK_STREAM, 0);
  if (cfd == -1) {
    printf("SOCKET ERROR: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sun_family = AF_LOCAL;
  strncpy(serv_addr.sun_path, ADDR, sizeof(ADDR));

  if (connect(cfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
    printf("CONNECT ERROR: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  strncpy(buff, "Hello server!", 14);
  send(cfd, buff, SIZE_BUFF, 0);

  // ожидаем сообщения от сервера
  recv(cfd, buff, SIZE_BUFF, 0);

  printf("%s\n", buff);

  // отправляем ответку

  close(cfd);

  return 0;
}
