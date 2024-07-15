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
  recvfrom(sfd, buff, SIZE_BUFF, 0, (struct sockaddr *)&cl_addr, &sock_len);

  printf("%s\n", buff);

  strncpy(buff, "Hello client!", 14);
  sendto(sfd, buff, SIZE_BUFF, 0, (struct sockaddr *)&cl_addr, sock_len);

  // ждем сообщение о получении
  recvfrom(sfd, buff, SIZE_BUFF, 0, (struct sockaddr *)&cl_addr, &sock_len);

  close(sfd);

  return 0;
}