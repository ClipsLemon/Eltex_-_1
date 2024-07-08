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
  while (1) {
    if ((queue_key = ftok(QUEUE_FILENAME, 1)) != -1) {
      if ((queue_id = msgget(queue_key, 0666)) != -1) {
        // ждем сообщения от сервера
        msgrcv(queue_id, &message_from_server, sizeof(message_from_server),
               SERVER, 0);
        printf("Message from server:%s\n", message_from_server.message_text);
        // после того как получили сообщение
        message_to_server.mtype = CLIENT;
        strncpy(message_to_server.message_text, "Hello!", 6);
        msgsnd(queue_id, &message_to_server, sizeof(message_to_server), 0);
        break;
      } else {
        printf("Ошибка подключения очереди.\nПовторное создание\n");
      }
    } else {
      printf("Ошибка создания ключа очереди.\nПовторное создание...\n");
    }
  }
  return 0;
}