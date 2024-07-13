#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  key_t key = ftok("server.c", 0);
  if (key == -1) {
    printf("ERROR: %s", strerror(errno));
    exit(1);
  }
  // создатель может читать и писать и все остальные тоже
  int shm_id =
      shmget(key, 64, IPC_CREAT | S_IWUSR | S_IRUSR | S_IROTH | S_IWOTH);
  if (shm_id == -1) {
    printf("ERROR: %s", strerror(errno));
    exit(1);
  }
  char *message = (char *)shmat(shm_id, NULL, 0);
  if (message == (void *)-1) {
    printf("ERROR: %s", strerror(errno));
    exit(1);
  }
  strncpy(message, "Hello client!", 14);

  // ждем от пользователя ответного сообщения
  while (strncmp(message, "Hello server!", 14)) {
    sleep(1);
  }
  printf("%s\n", message);
  shmdt(message);
  return 0;
}