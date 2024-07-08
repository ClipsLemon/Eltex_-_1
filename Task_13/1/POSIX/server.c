#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MESSAGE_SIZE 100
#define SERVER_PR 1
#define CLIENT_PR 2
#define QUEUE_SERVER "/queue_server"
#define QUEUE_CLIENT "/queue_client"

int main() {
  mqd_t mqdes_server;
  mqd_t mqdes_client;
  char message[256];
  unsigned int priority = SERVER_PR;
  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = 10;
  attr.mq_msgsize = 256;
  attr.mq_curmsgs = 0;

  mqdes_server =
      mq_open(QUEUE_SERVER, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR, &attr);
  mqdes_client =
      mq_open(QUEUE_CLIENT, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR, &attr);

  strncpy(message, "Hi!", 3);
  mq_send(mqdes_client, message, sizeof(message), CLIENT_PR);

  mq_receive(mqdes_server, message, sizeof(message), &priority);

  printf("Message from client: %s\n", message);

  mq_close(mqdes_client);
  mq_close(mqdes_server);

  mq_unlink(QUEUE_CLIENT);
  mq_unlink(QUEUE_SERVER);

  return 0;
}