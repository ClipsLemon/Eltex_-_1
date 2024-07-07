#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
  int p[2];
  char buf[80];
  pid_t pid;

  if (pipe(p)) {
    perror("ошибка канала");
    exit(1);
  }
  if ((pid = fork()) == 0) {
    close(p[1]);
    read(p[0], buf, sizeof(buf));
    printf("Сообщение дочернего процесса: %s\n", buf);
    exit(0);
  }
    close(p[0]);
    sprintf(buf, "Hi!");
    write(p[1], buf, strlen(buf) + 1);
    close(p[1]);
    

  exit(0);
}