#include "../main.h"

char **LineParser(char *program_line, char separator, int *len) {
  int argument_counter = 0;
  char **parsed_string = NULL;
  // переменная указывающая текущее положение на строке флага
  int flag_index = 0;

  // считаем общее количество разделителей, оно будет равно
  // количеству флагов: "ls -l -a" - два разделителя - два флага.
  for (int i = 0; i < PROGRAM_NAME_LEN + FLAGS_LEN && program_line[i] != '\0';
       i++) {
    if (program_line[i] == separator) {
      argument_counter++;
    }
  }

  // один под NULL и один по имя
  argument_counter += 2;

  *len = argument_counter;
  // выделяем память под двумерный массив чаров
  parsed_string = calloc(argument_counter, sizeof(char *));

  for (int i = 0; i < argument_counter; i++) {
    parsed_string[i] = calloc(FLAGS_LEN, 1);
  }

  for (int i = 0; i < PROGRAM_NAME_LEN + FLAGS_LEN && program_line[i] != '\0';
       i++) {
    // встретили разделитель - значит один аргумент уже записали
    if (program_line[i] == separator) {
      // проверяем, что последний и первый символ в строке не является пробелом
      if (parsed_string[*len - argument_counter][flag_index - 1] == ' ') {
        parsed_string[*len - argument_counter][flag_index - 1] = '\0';
      }
      argument_counter--;
      flag_index = 0;
    } else {
      // перед записью проверяем, что мы не пишем первый символ, если он равен
      // пробелу
      if (!(flag_index == 0 && program_line[i] == ' ')) {
        parsed_string[*len - argument_counter][flag_index] = program_line[i];
        flag_index++;
      }
    }
  }

  free(parsed_string[*len - 1]);
  parsed_string[*len - 1] = NULL;

  return parsed_string;
}