#include "color.h"
#include "server.h"

void QueueClose(mqd_t queue_id, char *queue_name) {
  int is_closed = 0;
  while (1) {
    // либо мы закрываем первы раз, либо не смогли отлинковать и отлинковываем
    // повторно
    if (is_closed || (mq_close(queue_id) == 0)) {
      is_closed = 1;
      if (mq_unlink(queue_name) == 0) {
        break;
      } else {
        printf(RED "ERROR: can\'t UNLINK queue: %s\n" END_COLOR, queue_name);
      }
    } else {
      printf(RED "ERROR: can\'t CLOSE queue: %s\n" END_COLOR, queue_name);
    }
  }

  printf(GREEN "LOG: A queue %s has been CLOSED successfully\n" END_COLOR,
         queue_name);
}
