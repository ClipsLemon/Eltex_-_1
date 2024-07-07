#include "../main.h"

void RemoveNewLineSymbol(char *string) {
  while (*string) {
    if (*string == '\n') {
      *string = '\0';
      break;
    }
    string++;
  }
}

void ConcatString(char *dst, char *src) {
  int len_src = 0;

  for (int i = 0; i < PROGRAM_NAME_LEN; i++) {
    if (!dst[i]) {
      len_src = i;
      break;
    }
  }

  for (int i = 0; i < PROGRAM_NAME_LEN; i++) {
    if (src[i]) {
      dst[i + len_src] = src[i];
    } else {
      dst[i + len_src] = '\0';
      break;
    }
  }
}

void ClearString(char *string, int len) {
  for (int i = 0; i < len; i++) {
    string[i] = '\0';
  }
}