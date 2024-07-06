#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define THREADS_COUNT 5

void* PrintThreadIndex(void* arg) { printf("Thread #: %d\n", *((int*)arg)); }
int main(void) {
  pthread_t threads[THREADS_COUNT];
  int index_array[THREADS_COUNT];

  for (int i = 0; i < THREADS_COUNT; i++) {
    index_array[i] = i;
    pthread_create(&threads[i], NULL, PrintThreadIndex, &index_array[i]);
  }
  for (int i = 0; i < THREADS_COUNT; i++) {
    pthread_join(threads[i], NULL);
  }
  return 0;
}