#include "../server.h"

void DeleteClient(char *clientname, Controller *info) {
  for (int i = 0; i < USERS_MAX; i++) {
    // ищем клииента к удалению
    if (strncmp(&info->user_list[i].username[0], &clientname[1],
                USERNAME_LEN) == 0) {
      info->user_list[i].last_message_index = -1;

      strncpy(info->chat_history[info->history_index].username, &clientname[1],
              USERNAME_LEN);
      strncpy(info->chat_history[info->history_index].message, "is exit", 8);
      info->history_index++;
      mq_send(info->mqdes_send, "upd", 4, 1);
      QueueClose(info->user_list[i].mqdes_client,
                 info->user_list[i].queue_name);
      break;
    }
  }
}