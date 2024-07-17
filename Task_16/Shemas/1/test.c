#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define CLIENT_NUM 10

void *ThreadCreateClient() {
  while (1) {
    system("./client");
    sleep(1);
  }
}

int main() {
  pthread_t client_array[CLIENT_NUM];
  for (int i = 0; i < CLIENT_NUM; i++) {
    pthread_create(&client_array[i], NULL, ThreadCreateClient, NULL);
  }

  while (1) {
    sleep(1);
  }
  return 0;
}