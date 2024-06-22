#include <stdio.h>

#define BYTE_SIZE 8
// указано с учетом хода от 0
#define CHANGED_BYTE 2
#define NO_ONE -1

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
 * @brief Функция создает маску, которую накладывает на исходное число. Если в
 * результате наложения  маски получается 0, то и бит, который надо проверить,
 * равен 0. Если после наложения маски число не равно 0, то бит равен 1.
 *
 * Пример (будет расписан только 1 байт для простоты):
 * передано число 5, надо проверить 3 бит
 * 5 : 00000101
 * маска выглядит так: 00000100 ( = 4)
 * При наложении маски остается 00000100 != 0. Значит взозвращается не 0, то
 * есть истина.
 * @param number - число, у которого смотрится бит
 * @param index - целевой бит
 * @return int
 */
int check_bit(const int *number, int index) {
  return ((*number) & (1 << index)) != 0;
}

/**
 * @brief Функция осуществляяет вывод заданного числа в двоичной системе
 * счисления. Мой процессор x64 от Intel работает на litle-endian формате записи
 * байтов. По этой причине функция разворачивает порядок байтов и битов для
 * вывода в привычной форме
 *
 * @param number - константный указатель на число
 * @param which_is_colored - показывает, какой байт будет выделен цветом. По
 * умолчанию NO_ONE
 */
void print_binary(const int *number, int which_is_colored) {
  for (int byte = (int)sizeof(int) - 1; byte >= 0; byte--) {
    if (which_is_colored == byte) {
      printf("\033[43m");
    }
    for (int bit = BYTE_SIZE - 1; bit >= 0; bit--) {
      printf("%d", check_bit(number, BYTE_SIZE * byte + bit));
    }
    printf("\033[0m");
    printf(" ");
  }
  printf("\n");
}

/**
 * @brief Функция обработки ввода. Нужна, для упрощения обработки ввода.
 * Записывает только целые положительные числа.
 * @param number - адрес на переменную, в которую будет осуществляться запись
 */
void input_number_handler(int *number) {
  while (1) {
    printf("Введите число: ");
    // если число считалось, то выходим из цилка
    if (scanf("%d", number)) {
      break;
    } else {
      printf("\033[31mСкорее всего, вы ввели строку! Введите число "
             "повторно.\033[0m\n");
      clear_input_buffer();
    }
  }
}

void set_bit(int *number, int bit) { ((*number) |= (1 << bit)); }

void reset_bit(int *number, int bit) { ((*number) &= ~(1 << bit)); }

int main() {
  // Замечание: можно было бы использовать unsigned int, однако тогда
  // пользователь сможет ввести отрицательное число. По условию задачи нужно
  // вводить только положительные.

  // вводимое изначальное число
  int a;
  // число, которое выступает в качестве третьего байта
  int third_byte;
  while (1) { // 1 while
    input_number_handler(&a);
    if (a < 0) {
      printf("\033[31mВы ввели отрицательное число! Введите число "
             "повторно.\033[0m\n");
    } else {
      // снова замкнутый цикл для корректного ввода числа
      while (1) { // 2 while
        printf("Введите число в диапазоне от 0 до 255.\n");
        input_number_handler(&third_byte);
        if (third_byte < 0 || third_byte > 255) {
          printf("\033[31mЛибо вы ввели отрицательное число, либо число больше "
                 "255! "
                 "Введите число повторно.\033[0m\n");
        } else {
          break; // 2 while exit
        }
      }
      printf("Изначальное число: \t\t");
      print_binary(&a, 2);
      printf("Число из которого берем байт: \t");
      print_binary(&third_byte, 0);

      for (int bit = 0; bit < BYTE_SIZE; bit++) {
        // сначала проверяем, задан ли бит
        if (check_bit(&third_byte, bit)) {
          // если задан, заменяем бит
          set_bit(&a, CHANGED_BYTE * BYTE_SIZE + bit);
          // если не задан, сбрасываем бит
        } else {
          reset_bit(&a, CHANGED_BYTE * BYTE_SIZE + bit);
        }
      }
      printf("Число после обработки: \t\t");
      print_binary(&a, 2);

      break; // 1 while exit
    }
  }
  return 0;
}