#include <stdio.h>

#define STRING_LEN 100

/**
 * @brief Функция возвращающая длину строки. Ищет до терминирующего нуля и
 * считает одновременно количество символов. Можно было бы использовать
 * стандартную strlen, но ради нее нужно подключать целую string.h, что в рамках
 * программы нецелесообразно.
 *
 * @param str - строка
 * @return size_t - длина строки
 */
size_t GetStrLen(const char *str) {
  const char *ptr_str = str;
  size_t len = 0;

  while (*(ptr_str++)) {
    len++;
  }

  return len;
}

/**
 * @brief Функция взятия строки. Используется метод fgets, после которого нужно
 * подчистить последний символ. По умолчанию, ввод прерывается после \n, после
 * которого и ставится \0. Однако из-за \n нет возможности провести адекватно
 * поиск подстроки. По этой причине необходимо игнорировать этот символ.
 *
 * @param string - адрес на строку, в которую осуществляется запись
 */
void GetString(char *string) {
  fgets(string, STRING_LEN, stdin);
  // нужно, чтобы избавиться от \n
  string[GetStrLen(string) - 1] = '\0';
}

/**
 * @brief Функция ищет вхождение подстроки в строку.
 * Алгоритм:
 * 1. Необходимо создать дополнительные указатели на строку и подстроку, чтобы
 * избежать смещения первоначального указателя на строку.
 * 2. Строка читается до терминирующего нуля.
 * 3. Зная длину подстроки, как только мы находим совпадение по первым символам,
 * уменьшаем переменную длины, чтобы понять, когда мы полностью просмотрим все
 * подстроку.
 * 4. Если мы не находим совпадающие символы, то возвращаем исходную длину
 * строки и ставим указатель подстроки на начало подстроки
 * 5. Если длина подстроки равна 0, значит до этого символы успешно прошли
 * проверку и мы нашли искомую подстроку. Выходим из цикла.
 * 6. Если после выхода из цикла длина строки не равна 0, значит подстроку не
 * нашли, ретерним NULL.
 * @param string - строка
 * @param substring - подстрока
 * @return char* адрес подстроки
 */
char *FindSubstring(const char *string, char *substring) {
  size_t substring_len = GetStrLen(substring);
  // копия нужна, чтобы не проводить лишние операции по взятию длины строки
  size_t cpy_len = substring_len;
  const char *ptr_string = string;
  const char *ptr_substring = substring;
  // ищем подстроку, только если задана не пустая строка и подстрока
  if (*ptr_substring && *ptr_string) {
    // читаем строку до терминирующего нуля
    while (*ptr_string) {
      if (*ptr_string == *ptr_substring) {
        substring_len--;
        ptr_substring++;
      } else {
        substring_len = cpy_len;
        ptr_substring = substring;
      }
      if (substring_len == 0) {
        ptr_string = ptr_string - cpy_len + 1;
        break;
      }
      ptr_string++;
    }
    if (substring_len != 0) {
      ptr_string = NULL;
    }
    // иначе нулл
  } else {
    ptr_string = NULL;
  }
  return (char *)ptr_string;
}

int main(void) {
  char string[STRING_LEN];
  char substring[STRING_LEN];

  printf("Введите строку (не более 100 символов): ");
  GetString(string);

  printf("Введите искомую подстроку (не более 100 символов): ");
  GetString(substring);

  char *index = FindSubstring(string, substring);

  printf("Вывод начиная с индекса искомой подстроки: %s\n", index);
  return 0;
}