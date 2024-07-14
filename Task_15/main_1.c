#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void siguser_handler(int signal) { printf("SIGUSER HANDLER: %d\n", signal); }

int main() {
  struct sigaction sg;
  sg.sa_flags = SA_RESTART;
  sg.sa_handler = siguser_handler;
  sigemptyset(&sg.sa_mask);

  sigaction(SIGUSR1, &sg, NULL);

  while (1) {
    sleep(1);
  }

  return 0;
}
