#include "../server.h"

/*
если не получилось добавить (все занято), то -1
иначе 1
*/
int AddClient(char *clientname) {
  int is_writed = -1;
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
      // отправляем сигнал на отправку сообщений
      mq_send(mqdes_send, "upd", 4, 1);
      break;
    }
  }
  return is_writed;
}