#include "../../calculator.h"

void ClearInputBuffer() {
  while ((getchar()) != '\n')
    ;
}

void InputNumberHandler(int *number) {
  while (1) {
    // если число считалось, то выходим из цилка
    if (!scanf("%d", number)) {
      printf("%sСкорее всего, вы ввели строку! Введите число "
             "повторно.%s\n", RED, END_COLOR);
      ClearInputBuffer();
    }else{
      break;
    }
  }
  // на случай, если пользователь ввел правильное число, но после будут буквы
  ClearInputBuffer();
}