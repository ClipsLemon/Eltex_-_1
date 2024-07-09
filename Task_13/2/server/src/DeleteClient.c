#include "../server.h"

/*
если не получилось добавить (все занято), то -1
иначе 1
*/
void DeleteClient(char *clientname) {
  for (int i = 0; i < USERS_MAX; i++) {
    // ищем клииента к удалению
    if (strncmp(user_list[i].username, &clientname[1], USERNAME_LEN) == 0) {
      user_list[i].last_message_index = -1;
      QueueClose(user_list[i].mqdes_client, user_list[i].queue_name);
      break;
    }
  }
}