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

void ClearInputBuffer() {
  // nodelay(win_chat_field, true);
  // nodelay(win_text_field, true);
  // nodelay(win_users_field, true);

  // cbreak();

  // nodelay(win_chat_field, false);
  // nodelay(win_text_field, false);
  // nodelay(win_users_field, false);

  // while ((wgetch(win_chat_field)) != '\n') {
  //   ;
  // }

  while ((wgetch(win_text_field)) != '\n') {
    ;
  }
  // while ((wgetch(win_users_field)) != '\n') {
  //   ;
  // }

  // nodelay(win_chat_field, false);
  // nodelay(win_text_field, false);
  // nodelay(win_users_field, false);
}