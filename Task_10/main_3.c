#include "main_3.h"

int main() {
  char program_line[PROGRAM_NAME_LEN + FLAGS_LEN];

  char exit_command[] = "exit";
  char **argument_array = NULL;
  int array_len = 0;
  int status = 0;
  int is_exit = 1;

  /*
  Программа ожидает ввода, после чего проверяет его, если пользователь ввел
  exit, то выходит. Иначе проверяет команду на наличие в системных путях
  */
  while (1) {
    printf("Введите программу вместе с ключами (например ls -la): ");
    fgets(program_line, PROGRAM_NAME_LEN + FLAGS_LEN, stdin);

    // подготавливаем данные
    RemoveNewLineSymbol(program_line);
    // проверка на "exit"
    for (int i = 0; i < 5; i++) {
      if (program_line[i] != exit_command[i]) {
        is_exit = 0;
        break;
      }
    }
    if (!is_exit) {
      // разделяем на пробелы
      argument_array = LineParser(program_line, &array_len);
      // находим программу
      FindProgram(argument_array);
      // дожидаемся окончания процесса
      wait(&status);
      // после чистим ее строку и память
      ClearString(program_line, PROGRAM_NAME_LEN + FLAGS_LEN);
      RemoveArgument(argument_array, array_len);
    } else {
      break;
    }
    is_exit = 1;
  }
  return 0;
}