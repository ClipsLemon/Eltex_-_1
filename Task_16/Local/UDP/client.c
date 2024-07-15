#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define SERV_ADDR "/local_server"
#define CLIENT_ADDR "/client"
#define LISTEN_BACKLOG 1
#define SIZE_BUFF 20

int main() {
  int cfd;
  struct sockaddr_un my_addr, serv_addr;
  socklen_t len_peer_addr = sizeof(serv_addr);
  char buff[SIZE_BUFF];
  memset(&my_addr, 0, sizeof(my_addr));
  memset(&serv_addr, 0, sizeof(serv_addr));

  my_addr.sun_family = AF_LOCAL;
  strncpy(my_addr.sun_path, CLIENT_ADDR, sizeof(CLIENT_ADDR));
  serv_addr.sun_family = AF_LOCAL;
  strncpy(serv_addr.sun_path, SERV_ADDR, sizeof(SERV_ADDR));

  cfd = socket(AF_LOCAL, SOCK_DGRAM, 0);

  if (access(CLIENT_ADDR, 0) == 0) {
    if (unlink(CLIENT_ADDR) == -1) {
      printf("UNLINK ERROR: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
    }
  }

  if (cfd == -1) {
    printf("SOCKET ERROR: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  if (bind(cfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1) {
    printf("BIND ERROR: %s\n", strerror(errno));
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
  unlink(CLIENT_ADDR);

  return 0;
}