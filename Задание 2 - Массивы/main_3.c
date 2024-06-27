#include <stdio.h>

/**
 * @brief Функция вывода массива.
 *
 * @param size размерность массива
 * @param array - константый указатель на массив, чтобы не было возможно его
 * изменить
 */
void PrintArray(int size, const int array[size][size]) {
  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size; c++) {
      printf("%d ", array[r][c]);
    }
    printf("\n");
  }
}

int main() {
  int n = 5;

  int array[n][n];
  for (int r = 0; r < n; r++) {
    for (int c = 0; c < n; c++) {
      if (c <= (n - (r + 2))) {
        array[r][c] = 0;
      } else {
        array[r][c] = 1;
      }
    }
  }
  PrintArray(n, array);
  return 0;
}