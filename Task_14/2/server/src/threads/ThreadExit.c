#include "../../../color.h"
#include "../../server.h"

void *ThreadExit() {
  char ext[5];
  printf(GREEN "EXIT THREAD HAS BEEN CREATED\n" END_COLOR);

  while (strncmp(ext, "exit", 5) != 0) {
    fgets(ext, 5, stdin);
  }
  printf(GREEN "EXIT THREAD HAS BEEN CLOSED\n" END_COLOR);

  return NULL;
}