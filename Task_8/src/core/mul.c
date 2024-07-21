#include "../../calculator.h"

int mul(){
  int first, second;
  printf("Введите первый множитель: ");
  InputNumberHandler(&first);
  printf("Введите второй множитель: ");
  InputNumberHandler(&second);
  return (first * second);
}