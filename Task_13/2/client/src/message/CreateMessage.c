#include "../../client.h"

void CreateMessage(Message *message, char *username) {
  // сообщение клиента
  time_t raw_time;
  struct tm *time_info;

  strncat(message->username, username, USERNAME_LEN);

  time(&raw_time);  // Получаем текущее время

  time_info = localtime(&raw_time);  // Преобразуем в локальное время

  // Форматируем время в строку
  strftime(message->datetime, DATE_TIME_SIZE, "%Y.%m.%d %H:%M", time_info);
}