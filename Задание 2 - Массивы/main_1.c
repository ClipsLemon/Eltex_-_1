#include <stdio.h>

/**
 * @brief Функция вывода массива.
 *
 * @param size размерность массива
 * @param array - константый указатель на массив, чтобы не было возможно его
 * изменить
 */
void print_array(int size, const int array[size][size]) {
  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size; c++) {
      printf("%4d ", array[r][c]);
    }
    printf("\n");
  }
}

int main() {
  int n = 6;

  int array[n][n];
  // пишем значения в массив только если матрица не одинарная
  for (int r = 0; r < n; r++) {
    for (int c = 0; c < n; c++) {
      array[r][c] = (r * n) + c + 1;
    }
  }
  print_array(n, array);
  return 0;
}