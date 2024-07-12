#include "../../color.h"
#include "../client.h"

void QueueDisconnect(mqd_t queue_id, char *queue_name) {
  while (1) {
    // либо мы закрываем первы раз, либо не смогли отлинковать и отлинковываем
    // повторно
    if ((mq_close(queue_id) != 0)) {
      printf("ERROR: can\'t CLOSE queue: %s\n", queue_name);
    } else {
      break;
    }
  }

  printf("LOG: A queue %s has been CLOSED successfully\n", queue_name);
}
