#include "../../color.h"
#include "../client.h"

void RemoveNewLineSymbol(char *string) {
  while (*string) {
    if (*string == '\n') {
      *string = '\0';
      break;
    }
    string++;
  }
}

void ClearString(char *string, int len) {
  for (int i = 0; i < len; i++) {
    string[i] = '\0';
  }
}

void GetName(char *string) {

  printf("Ограничение по длине %d символов.\n", USERNAME_LEN);
  while (1) {
    fgets(&string[1], USERNAME_LEN, stdin);
    if (strchr(&string[1], '%')) {
      printf(RED "В ващей строке запрещенный символ \'%%\'.\nВведите имя "
                 "заново.\n" END_COLOR);
    } else {
      RemoveNewLineSymbol(string);
      printf(GREEN "Приветствую %s!\n" END_COLOR, &string[1]);
      break;
    }
  }
}
