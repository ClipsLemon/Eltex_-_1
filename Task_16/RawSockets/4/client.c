#include <arpa/inet.h>
#include <errno.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/ether.h>
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

// запускался на ноутбуке

#define SERVER_IP "192.168.1.36"
#define CLIENT_IP "192.168.1.41"

#define SRC_MAC "c8:e2:65:08:41:75"
#define DST_MAC "04:7c:16:13:46:cf"

#define SOURCE_PORT 2049
#define SERV_PORT 2048
// 100 байт на payload
#define SIZE_BUFF 100
// длина заголовка udp
#define UDP_H_SIZE 8
// длина заголовка ip
// #define IP_H_SIZE 24
#define IP_H_SIZE 20

// длина заголовка ethernet
#define ETH_H_SIZE 14

#define TOTAL_LEN (SIZE_BUFF + UDP_H_SIZE + IP_H_SIZE + ETH_H_SIZE)

#define INTERFACE_NAME "wlp1s0"

short CalculateChecksum(struct iphdr *iph) {
  long tmp, csum = 0;
  unsigned short *ptr = (short *)iph;

  for (int i = 0; i < 10; ++i) {
    csum += *ptr;
    ++ptr;
  }

  while ((tmp = csum >> 16)) {
    csum = (csum & 0xFFFF) + tmp;
  }

  return ~csum;
}

void ErrorHandler(char *error_text) {
  printf(RED "%s: %s\n" END_COLOR, error_text, strerror(errno));
  // exit(EXIT_FAILURE);
}

int main() {
  int fd;
  struct sockaddr_ll server_addr, client_addr;
  socklen_t sock_len = sizeof(server_addr);
  int opt = 1;

  char buff[TOTAL_LEN];
  char buff_recv[TOTAL_LEN];

  struct ethhdr *eth_pack_header = (struct ethhdr *)buff;
  struct iphdr *ip_pack_header = (struct iphdr *)(buff + ETH_H_SIZE);
  struct udphdr *udp_pack_header =
      (struct updhdr *)(buff + ETH_H_SIZE + IP_H_SIZE);

  memset(buff, 0, SIZE_BUFF);
  fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  if (fd == -1) {
    ErrorHandler("SOCKET ERROR");
  }

  memset(&server_addr, 0, sizeof(server_addr));
  memset(&client_addr, 0, sizeof(client_addr));

  server_addr.sll_family = AF_PACKET;
  server_addr.sll_ifindex = if_nametoindex(INTERFACE_NAME);
  if (server_addr.sll_ifindex == 0) {
    ErrorHandler("INTERFACE ERROR");
  }
  server_addr.sll_halen = ETH_ALEN;

  // мак адрес компьютера
  // 04:7c:16:13:46:cf
  ether_aton_r(DST_MAC, (struct ether_addr *)server_addr.sll_addr);

  // заполняем ethhdr для назначения
  ether_aton_r(DST_MAC, (struct ether_addr *)eth_pack_header->h_dest);

  // заполняем ethhdr для источника
  ether_aton_r(SRC_MAC, (struct ether_addr *)eth_pack_header->h_source);

  eth_pack_header->h_proto = htons(ETH_P_IP);

  // заполняем ip
  ip_pack_header->version = 4;
  ip_pack_header->ihl = 5;
  ip_pack_header->protocol = IPPROTO_UDP;
  ip_pack_header->tot_len = htons(IP_H_SIZE + UDP_H_SIZE + SIZE_BUFF);
  ip_pack_header->frag_off = 0;
  ip_pack_header->ttl = 255;
  ip_pack_header->tos = 0;
  inet_pton(AF_INET, CLIENT_IP, &ip_pack_header->saddr);
  inet_pton(AF_INET, SERVER_IP, &ip_pack_header->daddr);

  ip_pack_header->check = CalculateChecksum(ip_pack_header);

  // Заполняем udp
  udp_pack_header->dest = htons(SERV_PORT);
  udp_pack_header->source = htons(SOURCE_PORT);
  udp_pack_header->len = htons(SIZE_BUFF + UDP_H_SIZE);
  udp_pack_header->check = 0;

  // пишем данные в payload
  strncpy(&buff[ETH_H_SIZE + IP_H_SIZE + UDP_H_SIZE], "Hello server!", 14);
  if (sendto(fd, buff, TOTAL_LEN, 0, (struct sockaddr *)&server_addr,
             sock_len) == -1) {
    ErrorHandler("SEND TO ERROR");
  }

  while (1) {
    if (recvfrom(fd, buff_recv, TOTAL_LEN, 0, (struct sockaddr *)&client_addr,
                 &sock_len) == -1) {
      ErrorHandler("RECV FROM ERROR");
    }

    udp_pack_header = (struct udphdr *)(buff_recv + ETH_H_SIZE + IP_H_SIZE);
    if (udp_pack_header->dest == htons(SOURCE_PORT)) {
      printf("MESSAGE FROM SERVER: %s\n",
             &buff_recv[ETH_H_SIZE + IP_H_SIZE + UDP_H_SIZE]);
      break;
    }
  }

  strncpy(&buff[ETH_H_SIZE + IP_H_SIZE + UDP_H_SIZE], "ACCEPT", 7);
  if (sendto(fd, buff, TOTAL_LEN, 0, (struct sockaddr *)&server_addr,
             sock_len) == -1) {
    ErrorHandler("SEND TO ERROR");
  }

  close(fd);
}