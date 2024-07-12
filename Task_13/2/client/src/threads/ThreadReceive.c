#include "../../../color.h"
#include "../../client.h"

/*
Принимает информацию от сервера в потоке клиента
*/
void *ThreadReceive(void *arg) {
  Controller *info = (Controller *)arg;

  // сообщение которое пришло пользователю
  Message tmp_message;
  unsigned int msg_priority = 0;
  // результат считывания сообщения
  ssize_t res = 0;

  fputs(GREEN "RECEIVE THREAD HAS BEEN CREATED\n" END_COLOR, info->log_file);
  while (1) {
    // смотрим
    if ((res = mq_receive(info->mqdes_server_msg, (char *)&tmp_message,
                          MESSAGE_PACK_LEN, &msg_priority)) != -1) {
      pthread_mutex_lock(&m1);
      // если дата пришла пустая, значит сервер отправил сообщение о входе или
      // выходе
      if (tmp_message.datetime[0] == '\0') {
        // если пользователь зашел, до добавляем его
        if (strncmp(tmp_message.message, "logged in", 10) == 0) {
          for (int i = 0; i < USERS_MAX; i++) {
            // находим первое свободное место и пишем туда клиента
            if (info->user_list[i][0] == '\0') {
              strncpy(info->user_list[i], tmp_message.username, USERNAME_LEN);
              break;
            }
          }
          // иначе пользователь выходит и его надо убрать
        } else {
          for (int i = 0; i < USERS_MAX; i++) {
            // находим клиента
            if (strncmp(info->user_list[i], tmp_message.username,
                        USERNAME_LEN) == 0) {
              memset(info->user_list[i], 0, USERNAME_LEN);
              break;
            }
          }
        }

        memcpy(&(info->chat_history[info->history_index]), &tmp_message,
               sizeof(tmp_message));
        PrintUsersList(info);
        info->history_index++;
        PrintChat(info);
        wclear(info->win_text_field);
        box(info->win_text_field, 0, 0);
        wrefresh(info->win_text_field);
      } else {
        memcpy(&(info->chat_history[info->history_index]), &tmp_message,
               sizeof(tmp_message));
        info->history_index++;
        PrintChat(info);
      }

      pthread_mutex_unlock(&m1);
    } else if (res == -1) {
      sleep(2);
    }
  }

  return NULL;
}
