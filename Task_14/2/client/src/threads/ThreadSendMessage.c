#include "../../client.h"

void *ThreadSendMessage(void *arg) {
  char log_msg[1024];
  Controller *info = (Controller *)arg;
  fputs("SEND MESSAGE THREAD HAS BEEN CREATED\n", info->log_file);
  char message[CL_MESSAGE_LEN];
  memset(message, 0, sizeof(message));
  int index = 0;
  // пока пользователь не введет выход
  while (1) {

    echo();
    mvwgetnstr(info->win_text_field, 2, 2, message, CL_MESSAGE_LEN);
    noecho();
    if (strncmp(message, "/exit", 6) == 0) {
      strncpy(info->chat->chat_history[info->chat->history_index].username,
              info->username, USERNAME_LEN);
      strncpy(info->chat->chat_history[info->chat->history_index].message,
              "logged out", 11);
      info->chat->history_index++;
      shutdown = 1;
      break;
    }
    PrintTextField(info);

    pthread_mutex_lock(&m1);
    sem_wait(info->send_message);

    CreateMessage(info, message);
    info->chat->history_index++;
    sem_post(info->send_message);
    pthread_mutex_unlock(&m1);
    PrintChat(info);
  }
  fputs("SEND MESSAGE THREAD HAS BEEN CLOSED\n", info->log_file);
}