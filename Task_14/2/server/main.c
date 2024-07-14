#include "../color.h"
#include "server.h"

int main() {
  // открываем разделяемую память
  printf(GREEN "SERVER START\n" END_COLOR);
  Controller *info;

  int shm_id = shm_open("messenger", DEFAULT_OFLGAS, DEFAULT_MODE);
  if (shm_id == -1) {
    printf(RED "ERROR shm_open: %s\n" END_COLOR, strerror(errno));
  }
  if (ftruncate(shm_id, 64) == -1) {
    printf("ERROR ftruncate: %s", strerror(errno));
    exit(1);
  }
  info = (Controller *)mmap(NULL, sizeof(Controller), PROT_READ | PROT_WRITE,
                            MAP_SHARED, shm_id, 0);
  if (info->chat_history == (void *)-1) {
    printf("ERROR mmap: %s", strerror(errno));
    exit(1);
  }

  info->history_index = 0;
  info->readed_index = 0;
  memset(&info->user_list, 0, sizeof(info->user_list));

  // создаем семафор на выключение
  sem_t *send_message;
  // создаем семафор на отправку сообщений
  send_message = sem_open(SEM_SEND_MESS, O_CREAT | O_RDWR, DEFAULT_MODE, 0);
  if (send_message == SEM_FAILED) {
    printf("SEMAPHORE ERROR: %s\n", strerror(errno));
    exit(0);
  }
  sem_post(send_message);

  // ожидает "exit" в терминале для выхода
  pthread_t exit_thread;

  pthread_create(&exit_thread, NULL, ThreadExit, NULL);

  pthread_join(exit_thread, NULL);

  sem_close(send_message);
  sem_unlink(SEM_SEND_MESS);

  printf(BLUE "LOG: server is closed\n" END_COLOR);

  return 0;
}