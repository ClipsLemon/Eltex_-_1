#include "../../main.h"

void CreateDir(char *path) {
  char foldername[FILENAME_LEN];

  chdir(path);
  // если не пустое имя
  GetString(foldername, "Enter foldername: ");
  if (strlen(foldername)) {
    // если файла не существует
    if (access(foldername, R_OK) != 0) {
      mkdir(foldername, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }
  }
}