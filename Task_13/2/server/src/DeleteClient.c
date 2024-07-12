#include "../server.h"

/*
если не получилось добавить (все занято), то -1
иначе 1
*/
void DeleteClient(char *clientname) {
  for (int i = 0; i < USERS_MAX; i++) {
    // ищем клииента к удалению
    if (strncmp(&user_list[i].username[0], &clientname[1], USERNAME_LEN) == 0) {
      user_list[i].last_message_index = -1;

      strncpy(chat_history[history_index].username, &clientname[1],
              USERNAME_LEN);
      strncpy(chat_history[history_index].message, "is exit", 8);
      history_index++;
      mq_send(mqdes_send, "upd", 4, 1);
      QueueClose(user_list[i].mqdes_client, user_list[i].queue_name);
      break;
    }
  }
}