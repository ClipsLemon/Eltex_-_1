#include "../../color.h"
#include "../client.h"

void QueueDisconnect(mqd_t queue_id, char *queue_name) {
  while (1) {
    // либо мы закрываем первы раз, либо не смогли отлинковать и отлинковываем
    // повторно
    if ((mq_close(queue_id) != 0)) {
      printf(RED "ERROR: can\'t CLOSE queue: %s\n" END_COLOR, queue_name);
    } else {
      break;
    }
  }

  printf(GREEN "LOG: A queue %s has been CLOSED successfully\n" END_COLOR,
         queue_name);
}
