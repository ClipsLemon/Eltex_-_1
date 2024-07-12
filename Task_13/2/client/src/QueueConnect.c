#include "../client.h"

mqd_t QueueConnect(char *queue_name, int oflag, mode_t mode, int message_len,
                   int number_of_messages, FILE *log_file) {
  mqd_t q_id;
  struct mq_attr attr;
  char string_to_file[1024];
  attr.mq_flags = 0;
  attr.mq_maxmsg = number_of_messages;
  attr.mq_msgsize = message_len;
  attr.mq_curmsgs = 0;

  while (1) {
    if ((q_id = mq_open(queue_name, oflag, mode, &attr)) == -1) {
      sprintf(string_to_file, "ERROR: can\'t CONNECT to queue: %s\n",
              queue_name);

      fputs(string_to_file, log_file);
      sprintf(string_to_file, "%s\n", strerror(q_id));
      fputs(string_to_file, log_file);
      sleep(1);
    } else {
      sprintf(string_to_file, "LOG: CONNECT to queue %s successfully\n",
              queue_name);
      fputs(string_to_file, log_file);
      break;
    }
  }

  return q_id;
}