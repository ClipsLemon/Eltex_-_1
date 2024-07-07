#include "../main_3.h"

void FindProgram(char **argument_array) {
  char path[PROGRAM_NAME_LEN] = BASE_BIN_DIR;
  int file_finded = 0;
  int child_pid = 0;
  ConcatString(path, argument_array[0]);

  // порождаем процесс в одном из двух случаев, если нашли в первом или втором
  // пути
  if (access(path, 0) == 0) {
    file_finded = 1;
  } else {
    /* попав в это условие, строка пути выглядит так: /bin/some_program
      поставив на пятый char '\0' можно будет заново провести конкатенацию
      путей
    */
    ClearString(path, PROGRAM_NAME_LEN);
    ConcatString(path, OPTIONAL_BIN_DIR);
    ConcatString(path, argument_array[0]);

    if (access(path, 0) == 0) {
      file_finded = 1;
    }
  }
  // если файл был найден, порождаем процесс
  if (file_finded) {
    child_pid = fork();
    if (child_pid == 0) {
      execv(path, argument_array);
      exit(1);
    }
  } else {
    printf("Такой программы не найдено!\n");
  }
}