#include "color.h"
#include <arpa/inet.h>
#include <errno.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// запускался на ноутбуке

#define SERVER_IP "192.168.1.36"
#define CLIENT_IP "192.168.1.41"

#define SERV_PORT 10000
#define SOURCE_PORT 12000
// 100 байт на payload
#define SIZE_BUFF 100
// длина заголовка udp
#define UDP_H_SIZE 8
// длина заголовка ip
#define IP_H_SIZE 20
// длина заголовка ethernet
#define ETH_H_SIZE 14

#define TOTAL_LEN (SIZE_BUFF + UDP_H_SIZE + IP_H_SIZE + ETH_H_SIZE)

#define INTERFACE_NAME "enp2s0"

// struct ethhdr {
//   uint8_t ether_dhost[6];
//   uint8_t ether_shost[6];
//   uint16_t ether_type;
// };

void CalculateCheckSum(struct iphdr *ip_header) {
  ip_header->check = 0;
  uint32_t check = 0;
  uint16_t *counter = (uint16_t *)ip_header;
  for (int i = 0; i < 10; i++) {
    check += counter[i];
  }
  while (check >> 16) {
    check = (check & 0xFFFF) + (check >> 16);
  }
  ip_header->check = ~check;
}

void ErrorHandler(char *error_text) {
  printf(RED "%s: %s\n" END_COLOR, error_text, strerror(errno));
  exit(EXIT_FAILURE);
}

int main() {
  int fd;
  struct sockaddr_ll server_addr, client_addr;
  socklen_t sock_len = sizeof(server_addr);
  int opt = 1;

  char buff[SIZE_BUFF + ETH_H_SIZE + IP_H_SIZE + UDP_H_SIZE];
  char buff_recv[SIZE_BUFF + ETH_H_SIZE + UDP_H_SIZE + IP_H_SIZE];

  struct ethhdr *eth_pack_header = (struct ethhdr *)buff;
  struct iphdr *ip_pack_header = (struct iphdr *)(buff + ETH_H_SIZE);
  struct udphdr *udp_pack_header =
      (struct updhdr *)(buff + ETH_H_SIZE + IP_H_SIZE);

  memset(buff, 0, SIZE_BUFF);

  fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  if (fd == -1) {
    ErrorHandler("SOCKET ERROR");
  }

  setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &opt, sizeof(opt));

  memset(&server_addr, 0, sizeof(server_addr));
  memset(&client_addr, 0, sizeof(client_addr));

  server_addr.sll_family = AF_PACKET;
  server_addr.sll_ifindex = if_nametoindex(INTERFACE_NAME);
  server_addr.sll_halen = 6;

  // мак адрес компьютера
  // 04:7c:16:13:46:cf
  server_addr.sll_addr[0] = 0x04;
  server_addr.sll_addr[1] = 0x7C;
  server_addr.sll_addr[2] = 0x16;
  server_addr.sll_addr[3] = 0x13;
  server_addr.sll_addr[4] = 0x46;
  server_addr.sll_addr[5] = 0xCF;

  // заполняем ethhdr для назначения
  eth_pack_header->h_dest[0] = 0x04;
  eth_pack_header->h_dest[1] = 0x7C;
  eth_pack_header->h_dest[2] = 0x16;
  eth_pack_header->h_dest[3] = 0x13;
  eth_pack_header->h_dest[4] = 0x46;
  eth_pack_header->h_dest[5] = 0xCF;

  // заполняем ethhdr для источника
  eth_pack_header->h_source[0] = 0xC8;
  eth_pack_header->h_source[1] = 0xE2;
  eth_pack_header->h_source[2] = 0x65;
  eth_pack_header->h_source[3] = 0x08;
  eth_pack_header->h_source[4] = 0x41;
  eth_pack_header->h_source[5] = 0x75;

  eth_pack_header->h_proto = htons(ETH_P_IP);

  // заполняем ip
  ip_pack_header->version = 4;
  ip_pack_header->ihl = 5;
  ip_pack_header->protocol = IPPROTO_UDP;
  ip_pack_header->tot_len = htons(IP_H_SIZE + UDP_H_SIZE + SIZE_BUFF);
  CalculateCheckSum(ip_pack_header);
  ip_pack_header->frag_off = 0;
  ip_pack_header->id = 0;
  ip_pack_header->ttl = 255;
  ip_pack_header->tos = 0;

  inet_pton(AF_INET, CLIENT_IP, &ip_pack_header->saddr);
  inet_pton(AF_INET, SERVER_IP, &ip_pack_header->daddr);

  // Заполняем udp
  udp_pack_header->dest = htons(SERV_PORT);
  udp_pack_header->source = htons(SOURCE_PORT);
  udp_pack_header->len = htons(SIZE_BUFF + UDP_H_SIZE);
  udp_pack_header->check = 0;

  // пишем данные в payload
  strncpy(&buff[IP_H_SIZE + UDP_H_SIZE], "Hello server!", 14);
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
      printf("%s\n", &buff_recv[ETH_H_SIZE + IP_H_SIZE + UDP_H_SIZE]);
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