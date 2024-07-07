#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(void){
  char *pipe_filename = "./pipe_fifo";
  char buf[80];

  int fd = open(pipe_filename, O_RDONLY);

  read(fd, buf, sizeof(buf));

  close(fd);
  printf("%s\n", buf);
  return 0;
}