#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "color.h"

#define S_IP_ADDR "127.0.0.1"
#define D_IP_ADDR "127.0.0.2"

#define SERV_PORT 10000
#define SOURCE_PORT 12000
// 100 байт на payload
#define SIZE_BUFF 100
// длина заголовка udp
#define UDP_H_SIZE 8
// длина заголовка ip
#define IP_H_SIZE 20

void ErrorHandler(char *error_text) {
  printf(RED "%s: %s\n" END_COLOR, error_text, strerror(errno));
  exit(EXIT_FAILURE);
}

int main() {
  int fd;
  struct sockaddr_in server_addr, client_addr;
  socklen_t sock_len = sizeof(server_addr);
  int opt = 1;

  char buff[SIZE_BUFF + IP_H_SIZE + UDP_H_SIZE];
  char buff_recv[SIZE_BUFF + UDP_H_SIZE + IP_H_SIZE];

  struct iphdr *ip_pack_header = (struct iphdr *)buff;
  struct udphdr *udp_pack_header = (struct updhdr *)(buff + IP_H_SIZE);

  memset(buff, 0, SIZE_BUFF);

  fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
  if (fd == -1) {
    ErrorHandler("SOCKET ERROR");
  }

  setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &opt, sizeof(opt));

  memset(&server_addr, 0, sizeof(server_addr));
  memset(&client_addr, 0, sizeof(client_addr));

  inet_pton(AF_INET, D_IP_ADDR, &server_addr.sin_addr.s_addr);
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(SERV_PORT);

  // заполняем ip
  ip_pack_header->version = 4;
  ip_pack_header->ihl = 5;
  ip_pack_header->protocol = IPPROTO_UDP;
  ip_pack_header->tot_len = htons(IP_H_SIZE + UDP_H_SIZE + SIZE_BUFF);
  ip_pack_header->check = 0;
  ip_pack_header->frag_off = 0;
  ip_pack_header->id = 0;
  ip_pack_header->ttl = 255;
  ip_pack_header->tos = 0;
  inet_pton(AF_INET, S_IP_ADDR, &ip_pack_header->saddr);
  inet_pton(AF_INET, D_IP_ADDR, &ip_pack_header->daddr);

  // Заполняем udp
  udp_pack_header->dest = htons(SERV_PORT);
  udp_pack_header->source = htons(SOURCE_PORT);
  udp_pack_header->len = htons(SIZE_BUFF + UDP_H_SIZE);
  udp_pack_header->check = 0;

  // пишем данные в payload
  strncpy(&buff[IP_H_SIZE + UDP_H_SIZE], "Hello server!", 14);
  if (sendto(fd, buff, SIZE_BUFF + IP_H_SIZE + UDP_H_SIZE, 0,
             (struct sockaddr *)&server_addr, sock_len) == -1) {
    ErrorHandler("SEND TO ERROR");
  }

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
  strncpy(&buff[IP_H_SIZE + UDP_H_SIZE], "ACCEPT", 7);
  if (sendto(fd, buff, SIZE_BUFF + UDP_H_SIZE + IP_H_SIZE, 0,
             (struct sockaddr *)&server_addr, sock_len) == -1) {
    ErrorHandler("SEND TO ERROR");
  }

  close(fd);
}