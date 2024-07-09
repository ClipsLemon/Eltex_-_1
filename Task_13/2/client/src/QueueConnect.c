#include "../../color.h"
#include "../client.h"

mqd_t QueueConnect(char *queue_name, int oflag, mode_t mode, int message_len,
                   int number_of_messages) {
  mqd_t q_id;
  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = number_of_messages;
  attr.mq_msgsize = message_len;
  attr.mq_curmsgs = 0;

  while (1) {
    if ((q_id = mq_open(queue_name, oflag, mode, &attr)) == -1) {
      printf(RED "ERROR: can\'t CONNECT to queue: %s\n" END_COLOR, queue_name);
      printf(RED "%s\n" END_COLOR, strerror(q_id));
    } else {
      printf(GREEN "LOG: CONNECT to queue %s successfully\n" END_COLOR,
             queue_name);
      break;
    }
  }

  return q_id;
}