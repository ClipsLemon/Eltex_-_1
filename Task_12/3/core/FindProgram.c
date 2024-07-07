#include "../main.h"

// возвращает путь программы если находит
void CheckProgram(char **programname, char *path) {
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
  if (!file_finded) {
    path = NULL;
  }
}

void FindProgram(char **program1, char **program2) {
  char path1[PROGRAM_NAME_LEN] = BASE_BIN_DIR;
  int child_pid = 0;
  int status = 0;

  CheckProgram(program1, path1);
  int p[2];
  char path2[PROGRAM_NAME_LEN] = BASE_BIN_DIR;
  if (program2 != NULL) {
    CheckProgram(program2, path2);
  }



  // если файл был найден, порождаем процесс
  if (path1) {
    printf("Программа была найдена = %s\n", path1);
    printf(" = %s\n", program1[0]);

    if (pipe(p)) {
      perror("ошибка канала");
      exit(1);
    }else{
      dup2(p[1], STDOUT_FILENO);
      child_pid = fork();
      if(child_pid == 0){
        execv(path1, program1);
        exit(1);
      }else{
        wait(&status);
        if (path2){
          dup2(p[0], STDIN_FILENO);
          child_pid = fork();
          if(child_pid == 0){
            execv(path2, program2);
            exit(1);
          }
          wait(&status);
        }
      }
    }
      
  } else {
    printf("Такой программы не найдено!\n");
  }
}