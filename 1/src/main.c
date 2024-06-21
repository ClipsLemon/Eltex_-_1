#include <stdio.h>

#define BYTE_SIZE 8

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
 * @param byte - байт, в котором находится целевой бит
 * @param bit - целевой бит
 * @return int
 */
int check_bit(const int *number, const int *byte, const int *bit) {
  return ((*number) & (1 << (*byte * BYTE_SIZE + *bit))) != 0;
}

/**
 * @brief Функция осуществляяет вывод заданного числа в двоичной системе
 * счисления. Мой процессор x64 от Intel работает на litle-endian формате записи
 * байтов. По этой причине функция разворачивает порядок байтов и битов для
 * вывода в привычной форме
 *
 * @param number - константный указатель на число
 */
void print_binary(const int *number) {
  for (int byte = (int)sizeof(int) - 1; byte >= 0; byte--) {
    for (int bit = BYTE_SIZE - 1; bit >= 0; bit--) {
      printf("%d", check_bit(number, &byte, &bit));
    }
    printf(" ");
  }
  printf("\n");
}

int main() {
  // Замечание: можно было бы использовать unsigned int, однако тогда
  // пользователь сможет ввести отрицательное число. По условию задачи нужно
  // вводить только положительные.
  int a;
  while (1) {
    printf("Введите число: ");
    if (scanf("%d", &a)) {
      if (a < 0) {
        printf("\033[31mВы ввели отрицательное число! Введите число "
               "повторно.\033[0m\n");
      } else {
        printf("Ваше число в двоичной системе счисления: ");
        print_binary(&a);
        break;
      }
    } else {
      printf("\033[31mСкорее всего, вы ввели строку! Введите число "
             "повторно.\033[0m\n");
      clear_input_buffer();
    }
  }

  return 0;
}
