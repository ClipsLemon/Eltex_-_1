#include "../../client.h"

void ParseServerMessage(char *message) {
  int index = 0;
  for (int i = 0; i < USERNAME_LEN && message[index] != '%'; i++, index++) {
    chat_history[history_index].username[i] = message[index];
  }
  index++;
  for (int i = 0; i < DATE_TIME_SIZE && message[index] != '%'; i++, index++) {
    chat_history[history_index].datetime[i] = message[index];
  }
  index++;
  for (int i = 0; i < CL_MESSAGE_LEN && message[index] != '\0'; i++, index++) {
    chat_history[history_index].message[i] = message[index];
  }
  history_index++;
}