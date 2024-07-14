#include "../../client.h"

void *ThreadFindUsers(void *arg) {
  Controller *info = (Controller *)arg;
  // последний прочитанный индекс, что бы лишний раз не бегать

  // ищем клиентов в списке сообщений
  while (shutdown == 0) {
    // ищем с последнего сообщения
    for (; info->chat->readed_index < info->chat->history_index;
         info->chat->readed_index++) {
      pthread_mutex_lock(&m1);
      // если нет даты, значит служебное сообщение
      if (info->chat->chat_history[info->chat->readed_index].datetime[0] ==
          '\0') {
        // если пользователь вошел, то добавляем его в список
        if (strncmp(info->chat->chat_history[info->chat->readed_index].message,
                    "logged in", 10) == 0) {
          // ищем ближайшее пустое место и добавляем его
          for (int i = 0; i < USERS_MAX; i++) {
            if (info->chat->user_list[i][0] == '\0') {
              strncpy(
                  info->chat->user_list[i],
                  info->chat->chat_history[info->chat->readed_index].username,
                  USERNAME_LEN);
              break;
            }
          }
          // иначе пользователь вышел и надо его убрать из списка
        } else {
          for (int i = 0; i < USERS_MAX; i++) {
            // находим такого клиента в списке
            if (strncmp(
                    info->chat->chat_history[info->chat->readed_index].username,
                    info->chat->user_list[i], USERNAME_LEN)) {
              info->chat->user_list[i][0] = '\0';
              break;
            }
          }
        }
      }
      pthread_mutex_unlock(&m1);
    }
    sleep(1);
  }

  return NULL;
}