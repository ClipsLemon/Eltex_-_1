#include "../server.h"

/*
если не получилось добавить (все занято), то -1
иначе 1
*/
int AddClient(char *clientname) {
  int is_writed = -1;
  int in_list = 0;
  // смотрим, был ли добавлен пользователь ранее
  for (int j = 0; j < USERS_MAX; j++) {
    if (strncmp(clientname, user_list[j].username, USERNAME_LEN) == 0) {
      in_list = 1;
    }
    // добавляем в исторю чата информацию о входе клиента
  }
  // елси пользователь не заходил, то записываем вход
  if (in_list == 0) {
    strncpy(chat_history[history_index].username, &clientname[1], USERNAME_LEN);
    strncpy(chat_history[history_index].message, "logged in", 10);
    history_index++;
    in_list = 1;
  }

  if (in_list == 1) {
    // TODO: проверку на наличие открытой очереди
    for (int i = 0; i < USERS_MAX; i++) {
      // ищем первое свободное место в списке клиентов
      if (user_list[i].last_message_index == -1) {

        user_list[i].last_message_index = 0;
        // копируем без служебной информации
        strncpy(user_list[i].username, &clientname[1], USERNAME_LEN);
        // формируем имя очеред
        strcpy(user_list[i].queue_name, "/");
        strncat(user_list[i].queue_name, &clientname[1], USERNAME_LEN);
        user_list[i].mqdes_client =
            QueueOpen(user_list[i].queue_name, DEFAULT_OFLGAS, DEFAULT_MODE,
                      MESSAGE_PACK_LEN, MAX_CL_MESSAGE);
        is_writed = 1;
        // проверяем, что этот клиент не входил до этого

        // отправляем сигнал на отправку сообщений

        break;
      }
    }
  }

  mq_send(mqdes_send, "upd", 4, 1);

  return is_writed;
}