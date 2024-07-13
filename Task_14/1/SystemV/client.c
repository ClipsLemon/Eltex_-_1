#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>

int main() {
  key_t key = ftok("server.c", 0);
  if (key == -1) {
    printf("ERROR: %s", strerror(errno));
    exit(1);
  }
  // клиент подключается
  int shm_id = shmget(key, 64, IPC_CREAT);
  if (shm_id == -1) {
    printf("ERROR: %s", strerror(errno));
    exit(1);
  }
  char *message = (char *)shmat(shm_id, NULL, 0);
  if (message == (void *)-1) {
    printf("ERROR: %s", strerror(errno));
    exit(1);
  }

  printf("%s\n", message);

  strncpy(message, "Hello server!", 14);

  shmdt(message);
  return 0;
}