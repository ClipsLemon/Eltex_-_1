#include "../main.h"

void AddAbonent(abonent **abonent_list) {
  // переменная копирует уже имеющуюся информацию
  abonent *copy = calloc(list_size + 1, sizeof(abonent));

  // обработка на исключительную ситуацию начала
  if (list_size != 0) {
    for (int i = 0; i < list_size; i++) {
      copy[i] = (*abonent_list)[i];
    }

    free(*abonent_list);
  }

  // записываем абонента в последний адрес
  WriteAbonent(&(copy[list_size]));

  // увеличваем размер листа на 1
  list_size++;

  // возвращаем данные
  (*abonent_list) = copy;

  // зануляем копию, так как на эти данные уже указывает оригинал
  copy = NULL;

  printf("\033[32mАбонент успешно записан!\033[0m\n");
}

void ShowAll(abonent **abonent_list) {
  for (int i = 0; i < list_size; i++) {
    // если не пустой, то выводим
    if (list_size != 0) {
      printf("____________________________\n");
      printf("Абонент № %d.\n", i + 1);
      printf("Имя: %s\n", (*abonent_list)[i].name);
      printf("Фамилия: %s\n", (*abonent_list)[i].second_name);
      printf("Телефон: %s\n", (*abonent_list)[i].tel);
    }
  }
}

void DeleteAbonent(abonent **abonent_list) {
  // признак удаления абонента
  int is_deleted = 0;
  char name[FIELD_SIZE];
  char second_name[FIELD_SIZE];
  abonent *copy = NULL;

  SetDefaultData(name);
  SetDefaultData(second_name);

  printf("Введите имя абонента: ");
  GetString(name);

  printf("Введите фамилию абонента: ");
  GetString(second_name);

  if (list_size != 1) {
    // переменная-счетчик для копирования. Если элемент находится по середине,
    // то он как бы делится на два, до и после элемента
    int counter = 0;
    for (int i = 0; i < list_size; i++) {
      // если имена совпали, но удаляем абонента
      if (StringCompare((*abonent_list)[i].name, name) &&
          StringCompare((*abonent_list)[i].second_name, second_name)) {
        copy = calloc(list_size - 1, sizeof(abonent));
        for (int j = 0; j < list_size; j++) {
          if (j != i) {
            copy[counter] = (*abonent_list)[j];
            counter++;
          }
        }
        list_size--;
        // чистим память
        free(*abonent_list);
        // переприсваеваем на новый список
        (*abonent_list) = copy;
        // зануляем указатель
        copy = NULL;
        is_deleted = 1;
        break;
      }
    }
  } else {
    free(*abonent_list);
    list_size--;
    is_deleted = 1;
  }

  if (is_deleted) {
    printf("\033[32mАбонент успешно удален!\033[0m\n");
  } else {
    printf("\033[31mАбонент по заданному имени не найден!\033[0m\n");
  }
}

void SearchByName(abonent **abonent_list) {
  int is_finded = 0;
  char name[FIELD_SIZE];
  char second_name[FIELD_SIZE];

  SetDefaultData(name);
  SetDefaultData(second_name);

  printf("Введите имя абонента: ");
  GetString(name);

  printf("Введите фамилию абонента: ");
  GetString(second_name);

  for (int i = 0; i < list_size; i++) {
    // если имена совпали, но выводим информацию об абоненте
    if (StringCompare((*abonent_list)[i].name, name) &&
        StringCompare((*abonent_list)[i].second_name, second_name)) {
      is_finded = 1;
      printf("\033[32mАбонент найден:\033[0m\n");
      printf("Абонент № %d.\n", i + 1);
      printf("Имя: %s\n", (*abonent_list)[i].name);
      printf("Фамилия: %s\n", (*abonent_list)[i].second_name);
      printf("Телефон: %s\n", (*abonent_list)[i].tel);

      break;
    }
  }
  if (!is_finded) {
    printf("\033[31mАбонент по заданному имени не найден!\033[0m\n");
  }
}