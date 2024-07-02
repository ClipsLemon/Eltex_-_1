#include "../main.h"

void PrintInterface(abonent *abonent_list) {
  int choice;
  int running = 1;
  while (running) {
    printf("\033[36m1) Добавить абонента\n\033[0m");
    printf("\033[36m2) Удалить абонента\n\033[0m");
    printf("\033[36m3) Поиск абонентов по имени\n\033[0m");
    printf("\033[36m4) Вывод всех записей\033[0m\n");
    printf("\033[36m5) Выход\033[0m\n");
    printf("\033[36mВаш выбор: \033[0m");

    InputNumberHandler(&choice);
    switch (choice) {
    case 1:
      AddAbonent(&abonent_list);

      break;
    case 2:
      DeleteAbonent(&abonent_list);
      break;
    case 3:
      SearchByName(&abonent_list);
      break;
    case 4:
      ShowAll(&abonent_list);
      break;
    default:
      if (list_size != 0) {
        free(abonent_list);
      }
      running = 0;
      break;
    }
  }
}