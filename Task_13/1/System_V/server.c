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
  struct msgbuf message_to_client;
  struct msgbuf message_from_client;
  while (1) {
    if ((queue_key = ftok(QUEUE_FILENAME, 1)) != -1) {
      if ((queue_id = msgget(queue_key, IPC_CREAT | 0666)) != -1) {
        message_to_client.mtype = SERVER;
        strncpy(message_to_client.message_text, "Hi!", 3);
        msgsnd(queue_id, &message_to_client, sizeof(message_to_client), 0);
        msgrcv(queue_id, &message_from_client, sizeof(message_from_client), CLIENT, 0);
        printf("Message from client:%s\n", message_from_client.message_text);
        while (1) {
          if (msgctl(queue_id, IPC_RMID, NULL) != -1) {
            break;
          }
        }
        break;
      } else {
        printf("Ошибка создания очереди.\nПовторное создание...\n");
      }
    } else {
      printf("Ошибка создания ключа очереди.\nПовторное создание...\n");
    }
  }
  return 0;
}