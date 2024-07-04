#include "../main.h"

void PathDeleteDir(char *path) {
  if (strcmp(path, "/")) {
    int last_slash = 0;
    for (int i = 0; path[i]; i++) {
      if (path[i] == '/') {
        last_slash = i;
      }
    }
    // если мы стоим на папке /home к примеру, то надо перейти в /, значит / не
    // должно быть затерто
    if (last_slash == 0) {
      last_slash++;
    }
    // идем от последнего слэша до конца строки
    for (int i = last_slash; path[i]; i++) {
      path[i] = '\0';
    }
  }
}

void PathAddDir(char *path, char *added_dir) {
  if (strcmp(path, "/") != 0) {
    strcat(path, "/");
  }
  strcat(path, added_dir);
}