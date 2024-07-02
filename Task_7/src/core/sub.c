#include "../../calculator.h"

int sub(){
  int first, second;
  printf("Введите уменьшаемое: ");
  InputNumberHandler(&first);
  printf("Введите вычитаемое: ");
  InputNumberHandler(&second);

  return (first - second);
}