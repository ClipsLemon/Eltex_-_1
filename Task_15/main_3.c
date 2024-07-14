#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  sigset_t sg;
  int signal;
  sigemptyset(&sg);
  sigaddset(&sg, SIGUSR1);
  sigprocmask(SIG_BLOCK, &sg, NULL);
  while (1) {
    sigwait(&sg, &signal);
    printf("GET BLOCKET SIGNAL: %d\n", signal);
  }
  return 0;
}
