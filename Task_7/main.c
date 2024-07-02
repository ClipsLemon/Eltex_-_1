#include "calculator.h"

int main(){
  int choice;
  int running = 1;
  while (running) {
    printf("%s1) Сложение\n", BLUE);
    printf("2) Вычитание\n");
    printf("3) Умножение\n");
    printf("4) Деление\n");
    printf("5) Выход\n");
    printf("Выберите операцию: %s\n", END_COLOR);

    InputNumberHandler(&choice);
    switch (choice) {
    case 1:
      printf("%sРезультат сложения: %d%s\n", GREEN, add(), END_COLOR);
      break;
    case 2:
      printf("%sРезультат вычитание: %d%s\n", GREEN, sub(), END_COLOR);
      break;
    case 3:
      printf("%sРезультат умножение: %d%s\n", GREEN, mul(), END_COLOR);
      break;
    case 4:
      printf("%sРезультат деления: %d%s\n", GREEN, div(), END_COLOR);
      break;
    default:
      running = 0;
      break;
    }
  }
  return 0;
}
