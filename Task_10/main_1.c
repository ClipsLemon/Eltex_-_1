#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  pid_t child_pid = fork();
  int status;
  if (child_pid == 0) {
    printf("Child pid   = %d\n", getpid());
    printf("Child ppid  = %d\n", getppid());
    exit(5);
  } else {
    printf("Parent pid  = %d\n", getpid());
    printf("Parent ppid = %d\n", getppid());
    wait(&status);
    printf("Status of child proccess = %d\n", WEXITSTATUS(status));
  }
  return 0;
}