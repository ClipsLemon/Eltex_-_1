#include "../main_3.h"

void RemoveArgument(char **argument_array, int len) {
  for (int i = 0; i < len - 1; i++) {
    free(argument_array[i]);
  }
  free(argument_array);
}