#include "../main.h"

void print_interface(abonent *abonent_list) {
  int choice;
  int running = 1;
  while (running) {
    printf("\033[36m1) Добавить абонента\n\033[0m");
    printf("\033[36m2) Удалить абонента\n\033[0m");
    printf("\033[36m3) Поиск абонентов по имени\n\033[0m");
    printf("\033[36m4) Вывод всех записей\033[0m\n");
    printf("\033[36m5) Выход\033[0m\n");
    printf("\033[36mВаш выбор: \033[0m");

    input_number_handler(&choice);
    switch (choice) {
      case 1:
        add_abonent(abonent_list);
        break;
      case 2:
        delete_abonent(abonent_list);
        break;
      case 3:
        search_by_name(abonent_list);
        break;
      case 4:
        show_all(abonent_list);
        break;
      default:
        running = 0;
        break;
    }
  }
}