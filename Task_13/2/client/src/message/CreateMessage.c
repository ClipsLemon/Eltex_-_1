#include "../../client.h"

void CreateMessage(char *message, char *username) {
  char message_text[CL_MESSAGE_LEN];
  char date_time[DATE_TIME_SIZE];

  time_t raw_time;
  struct tm *time_info;

  fgets(message_text, CL_MESSAGE_LEN, stdin);
  time(&raw_time); // Получаем текущее время

  time_info = localtime(&raw_time); // Преобразуем в локальное время

  // Форматируем время в строку
  strftime(date_time, DATE_TIME_SIZE, "%Y.%m.%d %H:%M", time_info);

  strncpy(message, username, USERNAME_LEN);
  strncat(message, "%%", 1);
  strncat(message, date_time, DATE_TIME_SIZE);
  strncat(message, "%%", 1);
  strncat(message, message_text, CL_MESSAGE_LEN);
}