#include "../../../color.h"
#include "../../server.h"

void *ThreadShutdown() {
  printf(GREEN "SHUTDOWN THREAD HAS BEEN CREATED\n" END_COLOR);
  char exit[5];
  int send_res = 0;
  while (1) {
    fgets(exit, 5, stdin);
    if (strncmp(exit, "exit", 5) == 0) {
      break;
    }
  }
  if ((send_res = mq_send(mqdes_shutdown, "1", 2, 1)) == -1) {
    printf(RED "ERROR SEND SHUTDOWN SIGNAL: %s\n" END_COLOR, strerror(errno));
  }
  return NULL;
}