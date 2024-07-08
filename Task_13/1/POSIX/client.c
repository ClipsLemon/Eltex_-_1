#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

#define QUEUE_FILENAME "server"
#define MESSAGE_SIZE 100
#define SERVER 1
#define CLIENT 2

struct msgbuf {
  long mtype;
  char message_text[MESSAGE_SIZE];
};

int main() {
  key_t queue_key;
  int queue_id;
  struct msgbuf message_from_server;
  struct msgbuf message_to_server;

  return 0;
}