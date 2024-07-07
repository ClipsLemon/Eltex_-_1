#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(void) {

  char *pipe_filename = "./pipe_fifo";
  // pid_t client_pid;

  mkfifo(pipe_filename, 0666);

  int fd = open(pipe_filename, O_WRONLY);
  write(fd, "Hi!", 4);

  close(fd);

  return 0;
}