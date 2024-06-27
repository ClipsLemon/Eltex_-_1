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
      printf("%4d ", array[r][c]);
    }
    printf("\n");
  }
}

int main() {
  int n = 5;

  int array[n][n];
  // значение точки спирали
  int num = 1;
  // переменные определяющие постепенно сужающиеся границы
  int top = 0, bottom = n - 1, left = 0, right = n - 1;

  /* Описание алгоритма:
      1. На каждом проходе цикла осуществляется заполнение сначала верхней
     строки, потом правого столбца, нижней строки и левого столбца
      2. После каждого такого заполнения граница сужается (либо ++ либо --).
      3. Как только наши границы сходятся, осуществляем последнюю итерацию и
     выходим из цикла.
  */
  while (top <= bottom && left <= right) {
    // Заполняется верхняя строка
    for (int i = left; i <= right; i++) {
      array[top][i] = num++;
    }
    top++;

    // заполняется правый столбец
    for (int i = top; i <= bottom; i++) {
      array[i][right] = num++;
    }
    right--;

    // Заполняется нижняя строка
    if (top <= bottom) {
      for (int i = right; i >= left; i--) {
        array[bottom][i] = num++;
      }
      bottom--;
    }

    // заполняется левый столбец
    if (left <= right) {
      for (int i = bottom; i >= top; i--) {
        array[i][left] = num++;
      }
      left++;
    }
  }

  PrintArray(n, array);
  return 0;
}