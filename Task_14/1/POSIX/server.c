#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  int shm_id = shm_open("server.c", O_CREAT | O_RDWR, S_IWUSR | S_IRUSR);
  if (shm_id == -1) {
    printf("ERROR shm_open: %s", strerror(errno));
    exit(1);
  }
  if (ftruncate(shm_id, 64) == -1) {
    printf("ERROR ftruncate: %s", strerror(errno));
    exit(1);
  }
  char *message =
      (char *)mmap(NULL, 64, PROT_READ | PROT_WRITE, MAP_SHARED, shm_id, 0);
  if (message == (void *)-1) {
    printf("ERROR mmap: %s", strerror(errno));
    exit(1);
  }
  strncpy(message, "Hello client!", 14);
  // ждем от пользователя ответного сообщения
  while (strncmp(message, "Hello server!", 14)) {
    sleep(1);
  }
  printf("%s\n", message);
  munmap(message, 64);
  shm_unlink("server.c");
  return 0;
}