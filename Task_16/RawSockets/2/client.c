#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "color.h"

#define IP_ADDR "127.0.0.1"
#define SERV_PORT 10000
#define SOURCE_PORT 12000
// 100 байт на payload и 8 на udp header
#define SIZE_BUFF 100
#define UDP_H_SIZE 8
#define IP_H_SIZE 20

void ErrorHandler(char *error_text) {
  printf(RED "%s: %s\n" END_COLOR, error_text, strerror(errno));
  exit(EXIT_FAILURE);
}

int main() {
  int fd;
  struct sockaddr_in server_addr, client_addr;
  socklen_t sock_len = sizeof(server_addr);

  char buff[SIZE_BUFF + UDP_H_SIZE];
  char buff_recv[SIZE_BUFF + UDP_H_SIZE + IP_H_SIZE];

  struct udphdr *udp_pack_header = (struct updhdr *)buff;

  memset(buff, 0, SIZE_BUFF);

  fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
  if (fd == -1) {
    ErrorHandler("SOCKET ERROR");
  }

  memset(&server_addr, 0, sizeof(server_addr));
  memset(&client_addr, 0, sizeof(client_addr));

  inet_pton(AF_INET, IP_ADDR, &server_addr.sin_addr.s_addr);
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(SERV_PORT);

  // специальная структура состоящая из двухбайтных полей
  udp_pack_header->dest = htons(SERV_PORT);
  udp_pack_header->source = htons(SOURCE_PORT);
  udp_pack_header->len = htons(SIZE_BUFF);
  udp_pack_header->check = 0;

  // пишем данные в payload
  strncpy(&buff[UDP_H_SIZE], "Hello server!", 14);

  if (sendto(fd, buff, SIZE_BUFF + UDP_H_SIZE, 0,
             (struct sockaddr *)&server_addr, sock_len) == -1) {
    ErrorHandler("SEND TO ERROR");
  }

  // recvfrom(fd, buff, SIZE_BUFF, 0, (struct sockaddr *)&server_addr,
  // &sock_len);
  while (1) {
    if (recvfrom(fd, buff_recv, SIZE_BUFF + UDP_H_SIZE + IP_H_SIZE, 0,
                 (struct sockaddr *)&client_addr, &sock_len) == -1) {
      ErrorHandler("RECV FROM ERROR");
    }
    udp_pack_header = (struct udphdr *)(buff_recv + IP_H_SIZE);
    if (udp_pack_header->dest == htons(SOURCE_PORT)) {
      printf("%s\n", &buff_recv[IP_H_SIZE + UDP_H_SIZE]);
      break;
    }
  }
  strncpy(&buff[UDP_H_SIZE], "ACCEPT", 7);
  if (sendto(fd, buff, SIZE_BUFF + UDP_H_SIZE, 0,
             (struct sockaddr *)&server_addr, sock_len) == -1) {
    ErrorHandler("SEND TO ERROR");
  }

  close(fd);
}