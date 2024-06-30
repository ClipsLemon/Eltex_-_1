#include "../main.h"

void ClearInputBuffer() {
  while ((getchar()) != '\n')
    ;
}

void InputNumberHandler(int *number) {
  while (1) {
    // если число считалось, то выходим из цилка
    if (scanf("%d", number)) {
      if (*number < 1 || *number > 5) {
        printf("\033[31mТакого пункта в меню нет. Введите цифру "
               "повторно.\033[0m\n");
      } else {
        break;
      }
    } else {
      printf("\033[31mСкорее всего, вы ввели строку! Введите число "
             "повторно.\033[0m\n");
      ClearInputBuffer();
    }
  }
  ClearInputBuffer();
}

void InitializationList(abonent *abonent_list) {
  for (int i = 0; i < LIST_SIZE; i++) {
    SetDefaultData(abonent_list[i].name);
    SetDefaultData(abonent_list[i].second_name);
    SetDefaultData(abonent_list[i].tel);
  }
}

void SetDefaultData(char *object_field) {
  for (int i = 0; i < FIELD_SIZE; i++) {
    object_field[i] = '\0';
  }
  // object_field[FIELD_SIZE - 1] = '\0';
}

int FindEmpy(const abonent *abonent_list) {
  // ближайший пустой объект справочника
  int index = -1;
  for (int i = 0; i < LIST_SIZE; i++) {
    // если объект справочника пустой, то берем индекс и выходим из цикла
    if (IsEmpty(&(abonent_list[i]))) {
      index = i;
      break;
    }
  }
  return index;
}

int IsEmpty(const abonent *abonent) {
  /* Конструкция для проверки полей была выбрана
 не просто так. Если у пользователя нет телефона, то эта структура не должна
 считаться пустой. Соответственно, если до этого хоть одно поле было не пустым,
 тогда и это поле не будет считаться пустым. Так же, если ошибочно пользователь
 не задал имя для абонента, тогда такая структура не будет засчитана как пустая,
 ведь после проверки на имя идет проверка на фамилию и телефон.
  */
  int code = 0;
  if (abonent->name[0] == '\0') {
    if (abonent->second_name[0] == '\0') {
      if (abonent->tel[0] == '\0') {
        code = 1;
      }
    }
  }
  return code;
}

size_t GetStrLen(const char *str) {
  const char *ptr_str = str;
  size_t len = 0;

  while (*(ptr_str++)) {
    len++;
  }

  return len;
}

void GetString(char *string) {
  fgets(string, FIELD_SIZE, stdin);
  int str_len = GetStrLen(string);
  // если мы считали строку и в ней есть \n, значит никаких лишних символов не
  // осталось и нам надо просто убрать \n из итоговой строки
  if (string[str_len - 1] == '\n') {
    string[str_len - 1] = '\0';
  } else {
    // в противном случае, если нет \n, значит от повис в буфере и надо его
    // очистить
    ClearInputBuffer();
  }
}

void WriteAbonent(abonent *writeble_abonent) {
  printf("Введите имя абонента: ");
  GetString(writeble_abonent->name);

  printf("Введите фамилию абонента: ");
  GetString(writeble_abonent->second_name);

  printf("Введите телефон абонента: ");
  GetString(writeble_abonent->tel);
}

int StringCompare(char *str1, char *str2) {
  int code = 1;
  if (*str1) {
    while (*str1 || *str2) {
      if (*str1 != *str2) {
        code = 0;
        break;
      }
      str1++;
      str2++;
    }
  } else {
    code = 0;
  }

  return code;
}