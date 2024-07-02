#include "../../calculator.h"

int add(){
  int first, second;
  printf("Введите первое слагаемое: ");
  InputNumberHandler(&first);
  printf("Введите второе слагаемое: ");
  InputNumberHandler(&second);

  return (first + second);
}