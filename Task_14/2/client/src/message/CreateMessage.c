#include "../../client.h"

void CreateMessage(Controller *info, char *message) {
  // сообщение клиента
  time_t raw_time;
  struct tm *time_info;
  int index = info->chat->history_index;

  strncat(info->chat->chat_history[index].username, info->username,
          USERNAME_LEN);

  time(&raw_time); // Получаем текущее время

  time_info = localtime(&raw_time); // Преобразуем в локальное время

  // Форматируем время в строку
  strftime(info->chat->chat_history[index].datetime, DATE_TIME_SIZE,
           "%Y.%m.%d %H:%M", time_info);

  // копируем сообщение
  strncpy(info->chat->chat_history[index].message, message, CL_MESSAGE_LEN);
}