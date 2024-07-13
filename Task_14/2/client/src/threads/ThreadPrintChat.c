#include "../../client.h"

void *ThreadPrintChat(void *arg) {
  Controller *info = (Controller *)arg;
  fputs("PRINT CHAT THREAD HAS BEEN CREATED\n", info->log_file);

  while (1) {
    pthread_mutex_lock(&m1);
    PrintChat(info);
    PrintUsersList(info);
    pthread_mutex_unlock(&m1);
    sleep(1);
  }

  fputs("PRINT CHAT THREAD HAS BEEN CLOSED\n", info->log_file);
}