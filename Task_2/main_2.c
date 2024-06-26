#include <stdio.h>

/**
 * @brief Функция очистки буфера. Можно использовать вместо самописной функции
 * fflush, однако он считается не безопасным из-за неполной поддержки различными
 * компиляторами и возможности потери данных.
 */
void ClearInputBuffer() {
  while ((getchar()) != '\n')
    ;
}

void InputArrayHandler(int *array_element) {
  while (1) {
    // если число считалось, то выходим из цилка
    if (scanf("%d", array_element)) {
      break;
    } else {
      printf(
          "\033[31mОдин из ваших элементов массива содержит строку. \nВведите "
          "новые данные начиная с этого элемента: \033[0m");
    }
    ClearInputBuffer();
  }
}

/**
 * @brief Функция вывода массива.
 *
 * @param size размерность массива
 * @param array - константый указатель на массив, чтобы не было возможно его
 * изменить
 */
void PrintArray(int size, int *array) {
  for (int r = 0; r < size; r++) {
    printf("%d ", array[r]);
  }
  printf("\n");
}

int main() {
  int n = 6;

  int array[n];
  printf("Размерность массива = %d\n", n);
  // пишем значения в массив только если матрица не одинарная
  printf("Введите данные массива через пробел: ");
  for (int r = 0; r < n; r++) {
    InputArrayHandler(array + r);
  }

  for (int r = 0; r < n / 2; r++) {
    array[r] ^= array[n - r - 1];
    array[n - r - 1] ^= array[r];
    array[r] ^= array[n - r - 1];
  }

  PrintArray(n, array);

  return 0;
}
