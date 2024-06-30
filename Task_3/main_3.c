#include <stdio.h>

#define ARRAY_SIZE 10

int main(void) {
  int array[ARRAY_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  for (int i = 0; i < ARRAY_SIZE; i++) {
    printf("%d ", *(array + i));
  }
  printf("\n");
  return 0;
}