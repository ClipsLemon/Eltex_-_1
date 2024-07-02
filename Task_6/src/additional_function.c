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

void SetDefaultData(char *object_field) {
  for (int i = 0; i < FIELD_SIZE; i++) {
    object_field[i] = '\0';
  }
  // object_field[FIELD_SIZE - 1] = '\0';
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
