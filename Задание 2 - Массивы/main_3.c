#include <stdio.h>

/**
 * @brief Функция очистки буфера. Можно использовать вместо самописной функции
 * fflush, однако он считается не безопасным из-за неполной поддержки различными
 * компиляторами и возможности потери данных.
 */
void clear_input_buffer() {
  while ((getchar()) != '\n')
    ;
}

/**
 * @brief Функция обработки ввода. Нужна, для упрощения обработки ввода.
 * Записывает только целые положительные числа.
 * @param number - адрес на переменную, в которую будет осуществляться запись
 */
void input_number_handler(int *number) {
  while (1) {
    printf("Введите размер массива: ");
    // если число считалось, то выходим из цилка
    if (scanf("%d", number)) {
      if (*number < 1) {
        printf("\033[31mВы ввели число меньше 1! Введите число "
               "повторно.\033[0m\n");
      } else {
        break;
      }
    } else {
      printf("\033[31mСкорее всего, вы ввели строку! Введите число "
             "повторно.\033[0m\n");
    }
    clear_input_buffer();
  }
}

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
      printf("%d ", array[r][c]);
    }
    printf("\n");
  }
}

int main() {
  int n;

  // считываем размерность массив от [1, INF)
  input_number_handler(&n);

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
  print_array(n, array);
  return 0;
}