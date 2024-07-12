#include "../server.h"

int AddClient(char *clientname, Controller *info) {
  int is_writed = -1;
  int in_list = 0;
  // смотрим, был ли добавлен пользователь ранее
  for (int j = 0; j < USERS_MAX; j++) {
    if (strncmp(clientname, info->user_list[j].username, USERNAME_LEN) == 0) {
      in_list = 1;
    }
    // добавляем в исторю чата информацию о входе клиента
  }
  // елси пользователь не заходил, то записываем вход
  if (in_list == 0) {
    strncpy(info->chat_history[info->history_index].username, &clientname[1],
            USERNAME_LEN);
    strncpy(info->chat_history[info->history_index].message, "logged in", 10);
    info->history_index++;
    in_list = 1;
  }

  if (in_list == 1) {
    // TODO: проверку на наличие открытой очереди
    for (int i = 0; i < USERS_MAX; i++) {
      // ищем первое свободное место в списке клиентов
      if (info->user_list[i].last_message_index == -1) {
        info->user_list[i].last_message_index = 0;
        // копируем без служебной информации
        strncpy(info->user_list[i].username, &clientname[1], USERNAME_LEN);
        // формируем имя очеред
        strcpy(info->user_list[i].queue_name, "/");
        strncat(info->user_list[i].queue_name, &clientname[1], USERNAME_LEN);
        info->user_list[i].mqdes_client =
            QueueOpen(info->user_list[i].queue_name, DEFAULT_OFLGAS,
                      DEFAULT_MODE, MESSAGE_PACK_LEN, MAX_CL_MESSAGE);
        is_writed = 1;
        // проверяем, что этот клиент не входил до этого

        // отправляем сигнал на отправку сообщений

        break;
      }
    }
  }

  mq_send(info->mqdes_send, "upd", 4, 1);

  return is_writed;
}