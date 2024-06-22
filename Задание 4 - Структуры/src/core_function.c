#include "../main.h"

void add_abonent(abonent *abonent_list) {
  // находим ближайшего пустого абонента
  int nearest_empty = find_empty(abonent_list);
  // если был найден ближайший пустой, значит есть место в справочнике
  if (nearest_empty >= 0) {
    write_abonent(&(abonent_list[nearest_empty]));
    // после того как был записан абонент, надо провести проверку на наличие
    // свободных мест в справочнике
    printf("\033[32mАбонент успешно записан!\033[0m\n");
    if (find_empty(abonent_list) == -1) {
      printf(
          "\033[33mДостигнут предел справочника!\nЗаполнены все %d "
          "абонентов.\033[0m\n",
          LIST_SIZE);
    }
  } else {
    printf(
        "\033[31mМеста в справочнике больше нет!\nПожалуйста, удалите "
        "одного из "
        "абонентов.\033[0m\n");
  }
}
// void delete_abonent();
// void search_by_name();
void show_all(abonent *abonent_list) {
  for (int i = 0; i < LIST_SIZE; i++) {
    // если не пустой, то выводим
    if (!is_empty(&(abonent_list[i]))) {
      printf("____________________________\n");
      printf("Абонент № %d.\n", i + 1);
      printf("Имя: %s\n", abonent_list[i].name);
      printf("Фамилия: %s\n", abonent_list[i].second_name);
      printf("Телефон: %s\n", abonent_list[i].tel);
    }
  }
}

void delete_abonent(abonent *abonent_list) {
  // признак удаления абонента
  int is_deleted = 0;
  char name[FIELD_SIZE];
  char second_name[FIELD_SIZE];

  set_default_data(name);
  set_default_data(second_name);

  printf("Введите имя абонента: ");
  get_string(name);

  printf("Введите фамилию абонента: ");
  get_string(second_name);

  for (int i = 0; i < LIST_SIZE; i++) {
    // если имена совпали, но зануляем абонента
    if (string_compare(abonent_list[i].name, name) &&
        string_compare(abonent_list[i].second_name, second_name)) {
      set_default_data(abonent_list[i].name);
      set_default_data(abonent_list[i].second_name);
      set_default_data(abonent_list[i].tel);

      is_deleted = 1;

      break;
    }
  }
  if (is_deleted) {
    printf("\033[32mАбонент успешно удален!\033[0m\n");
  } else {
    printf("\033[31mАбонент по заданному имени не найден!\033[0m\n");
  }
}

void search_by_name(abonent *abonent_list) {
  int is_finded = 0;
  char name[FIELD_SIZE];
  char second_name[FIELD_SIZE];

  set_default_data(name);
  set_default_data(second_name);

  printf("Введите имя абонента: ");
  get_string(name);

  printf("Введите фамилию абонента: ");
  get_string(second_name);

  for (int i = 0; i < LIST_SIZE; i++) {
    // если имена совпали, но выводим информацию об абоненте
    if (string_compare(abonent_list[i].name, name) &&
        string_compare(abonent_list[i].second_name, second_name)) {
      is_finded = 1;
      printf("\033[32mАбонент найден:\033[0m\n");
      printf("Абонент № %d.\n", i + 1);
      printf("Имя: %s\n", abonent_list[i].name);
      printf("Фамилия: %s\n", abonent_list[i].second_name);
      printf("Телефон: %s\n", abonent_list[i].tel);

      break;
    }
  }
  if (!is_finded) {
    printf("\033[31mАбонент по заданному имени не найден!\033[0m\n");
  }
}