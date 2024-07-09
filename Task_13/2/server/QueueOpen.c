#include "color.h"
#include "server.h"

mqd_t QueueOpen(char *queue_name, int oflag, mode_t mode,
                struct mq_attr *attr) {
  mqd_t q_id;
  while (1) {
    if ((q_id = mq_open(queue_name, oflag, mode, attr)) == -1) {
      printf(RED "ERROR: can\'t CREATE queue: %s\n" END_COLOR, queue_name);
    } else {
      printf(GREEN "LOG: A queue %s has been CREATED successfully\n" END_COLOR,
             queue_name);
      break;
    }
  }

  return q_id;
}