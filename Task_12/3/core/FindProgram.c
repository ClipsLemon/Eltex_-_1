#include "../main.h"

// возвращает путь программы если находит
char *CheckProgram(char **programname, char *path) {
  int file_finded = 0;
  // если у второго аргумента программа есть

  ConcatString(path, programname[0]);
  if (access(path, 0) == 0) {
    file_finded = 1;
  } else {
    // если с тем путем не вышло, то смотрим на /bin/usr/
    ClearString(path, PROGRAM_NAME_LEN);
    ConcatString(path, OPTIONAL_BIN_DIR);
    ConcatString(path, programname[0]);
    if (access(path, 0) == 0) {
      file_finded = 1;
    }
  }
  if (file_finded) {
    return path;
  } else {
    return NULL;
  }
}

void FindProgram(char **program1, char **program2) {
  char path1[PROGRAM_NAME_LEN] = BASE_BIN_DIR;
  CheckProgram(program1, path1);

  char path2[PROGRAM_NAME_LEN] = BASE_BIN_DIR;
  if (program2 != NULL) {
    CheckProgram(program2, path2);
  }

  // если файл был найден, порождаем процесс
  if (path1) {
    printf("Программа была найдена = %s\n", path1);
    printf(" = %s\n", program1[0]);

    // child_pid = fork();
    // if (child_pid == 0) {
    //   execv(path, argument_array1);

    //   exit(1);
    // }
  } else {
    printf("Такой программы не найдено!\n");
  }
}