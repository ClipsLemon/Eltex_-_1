#include "../../color.h"
#include "../server.h"

mqd_t QueueOpen(char *queue_name, int oflag, mode_t mode, int message_len,
                int number_of_messages) {
  mqd_t q_id;
  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = number_of_messages;
  attr.mq_msgsize = message_len;
  attr.mq_curmsgs = 0;

  while (1) {
    if ((q_id = mq_open(queue_name, oflag, mode, &attr)) == -1) {
      printf(RED "ERROR: can\'t CREATE queue: %s\n" END_COLOR, queue_name);
    } else {
      printf(GREEN "LOG: A queue %s has been CREATED successfully\n" END_COLOR,
             queue_name);
      break;
    }
  }

  return q_id;
}