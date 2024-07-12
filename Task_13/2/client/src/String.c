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

void GetName(char *string) {
  printf("Ограничение по длине %d символов.\n", USERNAME_LEN);
  while (1) {
    fgets(&string[1], USERNAME_LEN, stdin);
    if (strchr(&string[1], '%')) {
      printf(
          "В ващей строке запрещенный символ \'%%\'.\nВведите имя "
          "заново.\n");
    } else {
      RemoveNewLineSymbol(string);
      printf("Приветствую %s!\n", &string[1]);
      break;
    }
  }
}
