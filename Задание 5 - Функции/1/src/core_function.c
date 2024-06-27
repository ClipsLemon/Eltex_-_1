#include "../main.h"

void AddAbonent(abonent *abonent_list) {
  // находим ближайшего пустого абонента
  int nearest_empty = FindEmpy(abonent_list);
  // если был найден ближайший пустой, значит есть место в справочнике
  if (nearest_empty >= 0) {
    WriteAbonent(&(abonent_list[nearest_empty]));
    // после того как был записан абонент, надо провести проверку на наличие
    // свободных мест в справочнике
    printf("\033[32mАбонент успешно записан!\033[0m\n");
    if (FindEmpy(abonent_list) == -1) {
      printf("\033[33mДостигнут предел справочника!\nЗаполнены все %d "
             "абонентов.\033[0m\n",
             LIST_SIZE);
    }
  } else {
    printf("\033[31mМеста в справочнике больше нет!\nПожалуйста, удалите "
           "одного из "
           "абонентов.\033[0m\n");
  }
}
// void DeleteAbonent();
// void SearchByName();
void ShowAll(abonent *abonent_list) {
  for (int i = 0; i < LIST_SIZE; i++) {
    // если не пустой, то выводим
    if (!IsEmpty(&(abonent_list[i]))) {
      printf("____________________________\n");
      printf("Абонент № %d.\n", i + 1);
      printf("Имя: %s\n", abonent_list[i].name);
      printf("Фамилия: %s\n", abonent_list[i].second_name);
      printf("Телефон: %s\n", abonent_list[i].tel);
    }
  }
}

void DeleteAbonent(abonent *abonent_list) {
  // признак удаления абонента
  int is_deleted = 0;
  char name[FIELD_SIZE];
  char second_name[FIELD_SIZE];

  SetDefaultData(name);
  SetDefaultData(second_name);

  printf("Введите имя абонента: ");
  GetString(name);

  printf("Введите фамилию абонента: ");
  GetString(second_name);

  for (int i = 0; i < LIST_SIZE; i++) {
    // если имена совпали, но зануляем абонента
    if (StringCompare(abonent_list[i].name, name) &&
        StringCompare(abonent_list[i].second_name, second_name)) {
      SetDefaultData(abonent_list[i].name);
      SetDefaultData(abonent_list[i].second_name);
      SetDefaultData(abonent_list[i].tel);

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

void SearchByName(abonent *abonent_list) {
  int is_finded = 0;
  char name[FIELD_SIZE];
  char second_name[FIELD_SIZE];

  SetDefaultData(name);
  SetDefaultData(second_name);

  printf("Введите имя абонента: ");
  GetString(name);

  printf("Введите фамилию абонента: ");
  GetString(second_name);

  for (int i = 0; i < LIST_SIZE; i++) {
    // если имена совпали, но выводим информацию об абоненте
    if (StringCompare(abonent_list[i].name, name) &&
        StringCompare(abonent_list[i].second_name, second_name)) {
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