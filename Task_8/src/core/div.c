#include "../../calculator.h"

int div(){
  int first, second;
  printf("Введите делимое: ");
  InputNumberHandler(&first);
  while(1){
  printf("Введите делитель: ");
  InputNumberHandler(&second);
  if(second == 0){
    printf("%sНа 0 делить нельзя.%s\n", RED, END_COLOR);
  }else{
    break;
  }
  }

  return (first / second);
}