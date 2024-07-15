#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define ADDR "/local_server"
#define LISTEN_BACKLOG 1

#define SIZE_BUFF 20

int main() {
  int sfd, cfd;
  struct sockaddr_un my_addr, peer_addr;
  int len_peer_addr;

  char buff[SIZE_BUFF];
  memset(buff, 0, SIZE_BUFF);

  // если в прошлый раз сервер был закрыт некорректно
  if (access(ADDR, 0) == 0) {
    if (unlink(ADDR) == -1) {
      printf("UNLINK ERROR: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
    }
  }

  sfd = socket(AF_LOCAL, SOCK_STREAM, 0);
  if (sfd == -1) {
    printf("SOCKET ERROR: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  memset(&my_addr, 0, sizeof(my_addr));
  my_addr.sun_family = AF_LOCAL;
  strncpy(my_addr.sun_path, ADDR, sizeof(ADDR));

  if (bind(sfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1) {
    printf("BIND ERROR: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  if (listen(sfd, LISTEN_BACKLOG) == -1) {
    printf("LISTEN ERROR: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  cfd = accept(sfd, (struct sockaddr *)&peer_addr, &len_peer_addr);

  // ожидаем сообщения от клиента
  recv(cfd, buff, SIZE_BUFF, 0);

  printf("%s\n", buff);

  // отправляем ответку
  strncpy(buff, "Hi!", 4);
  send(cfd, buff, SIZE_BUFF, 0);

  // ждем сообщения о том, что клиент получил наше сообщение
  recv(cfd, buff, SIZE_BUFF, 0);
  close(sfd);
  close(cfd);

  unlink(ADDR);

  return 0;
}
