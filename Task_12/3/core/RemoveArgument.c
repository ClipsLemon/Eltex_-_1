#include "../main.h"

void RemoveArgument(char **argument_array, int len) {
  for (int i = 0; i < len; i++) {
    free(argument_array[i]);
  }
  free(argument_array);
}