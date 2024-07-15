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
  int sfd;
  struct sockaddr_un my_addr, peer_addr;
  socklen_t len_peer_addr = sizeof(peer_addr);
  char buff[SIZE_BUFF];
  memset(&my_addr, 0, sizeof(my_addr));
  memset(&peer_addr, 0, sizeof(peer_addr));

  // если в прошлый раз сервер был закрыт некорректно
  if (access(ADDR, 0) == 0) {
    if (unlink(ADDR) == -1) {
      printf("UNLINK ERROR: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
    }
  }

  sfd = socket(AF_LOCAL, SOCK_DGRAM, 0);
  if (sfd == -1) {
    printf("SOCKET ERROR: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
  my_addr.sun_family = AF_LOCAL;
  strncpy(my_addr.sun_path, ADDR, sizeof(ADDR));

  if (bind(sfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1) {
    printf("BIND ERROR: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  // ждем сообщение от клиента
  recvfrom(sfd, buff, SIZE_BUFF, 0, (struct sockaddr *)&peer_addr,
           &len_peer_addr);

  printf("%s\n", buff);

  strncpy(buff, "Hello client!", 14);
  sendto(sfd, buff, SIZE_BUFF, 0, (struct sockaddr *)&peer_addr, len_peer_addr);

  // ждем сообщение о получении
  recvfrom(sfd, buff, SIZE_BUFF, 0, (struct sockaddr *)&peer_addr,
           &len_peer_addr);

  close(sfd);

  unlink(ADDR);

  return 0;
}