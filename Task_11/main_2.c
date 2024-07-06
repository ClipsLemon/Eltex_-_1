#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define TOTAL_NUMBER 180000000
#define NUMBER_OF_THREADS 10
#define NUMBER_FOR_THREAD (TOTAL_NUMBER / NUMBER_OF_THREADS)

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
int number = 0;

void *ThreadCalc() {
  int i;
  for (int i = 0; i < NUMBER_FOR_THREAD; i++) {
    pthread_mutex_lock(&m1);
    number++;
    pthread_mutex_unlock(&m1);
  }
}

int main(void) {
  pthread_t threads[NUMBER_OF_THREADS];

  for (int i = 0; i < NUMBER_OF_THREADS; i++) {
    pthread_create(&threads[i], NULL, ThreadCalc, NULL);
  }

  printf("calculating...\n");

  for (int i = 0; i < NUMBER_OF_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("number = %d\n", number);
}