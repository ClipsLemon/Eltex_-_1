#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>

int main() {
  key_t key = ftok("server.c", 0);
  // клиент подключается
  int shm_id = shmget(key, 64, IPC_CREAT);
  char *message = (char *)shmat(shm_id, NULL, 0);

  printf("%s\n", message);

  strncpy(message, "Hello server!", 14);

  shmdt(message);
  return 0;
}