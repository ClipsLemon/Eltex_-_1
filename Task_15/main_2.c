#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
  sigset_t sg;
  sigemptyset(&sg);
  sigaddset(&sg, SIGINT);
  sigprocmask(SIG_BLOCK, &sg, NULL);
  while(1){
    sleep(1);
  }
  return 0;
}
