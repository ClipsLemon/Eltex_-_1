#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  pid_t child_pid_1;
  pid_t child_pid_2;
  pid_t child_pid_3;
  pid_t child_pid_4;
  pid_t child_pid_5;
  int status_1;
  int status_2;
  int status_3;
  int status_4;
  int status_5;

  child_pid_1 = fork();
  if (child_pid_1 == 0) {
    printf("Process №1 pid  = %d\n", getpid());
    printf("Process №1 ppid = %d\n", getppid());
    child_pid_3 = fork();
    if (child_pid_3 == 0) {
      printf("Process №3 pid  = %d\n", getpid());
      printf("Process №3 ppid = %d\n", getppid());
      exit(3);
    } else {
      child_pid_4 = fork();
      if (child_pid_4 == 0) {
        printf("Process №4 pid  = %d\n", getpid());
        printf("Process №4 ppid = %d\n", getppid());
        exit(4);
      }
    }
    waitpid(child_pid_3, &status_3, 0);
    waitpid(child_pid_4, &status_4, 0);
    printf("Status of child proccess №3 = %d\n", WEXITSTATUS(status_3));
    printf("Status of child proccess №4 = %d\n", WEXITSTATUS(status_4));
    exit(1);
  } else {
    child_pid_2 = fork();
    if (child_pid_2 == 0) {
      printf("Process №2 pid  = %d\n", getpid());
      printf("Process №2 ppid = %d\n", getppid());
      child_pid_5 = fork();
      if (child_pid_5 == 0) {
        printf("Process №5 pid  = %d\n", getpid());
        printf("Process №5 ppid = %d\n", getppid());
        exit(5);
      }
      waitpid(child_pid_5, &status_5, 0);
      printf("Status of child proccess №5 = %d\n", WEXITSTATUS(status_5));
      exit(2);
    }
  }

  waitpid(child_pid_1, &status_1, 0);
  waitpid(child_pid_2, &status_2, 0);

  printf("Parrent pid  = %d\n", getpid());
  printf("Parrent ppid = %d\n", getppid());
  printf("Status of child proccess №1 = %d\n", WEXITSTATUS(status_1));
  printf("Status of child proccess №2 = %d\n", WEXITSTATUS(status_2));

  return 0;
}