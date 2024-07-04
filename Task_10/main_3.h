#ifndef MAIN_3_H
#define MAIN_3_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// здесь в линуксе хранится масса основных утилит
#define BASE_BIN_DIR "/bin/"

// тут хранятся пользовательские утилиты, например firefox вызывается из этой
// папки
#define OPTIONAL_BIN_DIR "/usr/bin/"
#define PROGRAM_NAME_LEN 100
#define FLAGS_LEN 50

/**
 * @brief Функция разделяет входную строку по пробелам
 *
 * @param program_line - исходная строка
 * @return char** - массив указателей на строки, где каждый элемент - "слово"
 */
char **LineParser(char *program_line, int *len);

/**
 * @brief Функция находит символ \n и заменяет на '\0'
 *
 * @param string - строка
 */
void RemoveNewLineSymbol(char *string);
/**
 * @brief Функция получает на вход двумерный массив аргументов.
 * 0 аргумент - обязательно название программы
 * 1 и далее аргументы - опции команды
 *
 * @param argument_array - массив указателей на строки с распарсенным
 * аргументами
 */
/**
 * @brief Функция получает на вход двумерный массив аргументов.
 * 0 аргумент - обязательно название программы
 * 1 и далее аргументы - опции команды
 *
 * @param argument_array - массив указателей на строки с распарсенным
 * аргументами
 */
void FindProgram(char **argument_array);

/**
 * @brief Функция конкатенации. Пишет начиная с '\0' dst и после конкатенации
 * ставит '\0'
 *
 * @param dst - куда происходит конкатенация
 * @param src - что конкатенируем
 */
void ConcatString(char *dst, char *src);

/**
 * @brief Функция чистит память динамического массива аргументов
 *
 * @param argument_array - динамический массив аргументов
 * @param len - длина массива
 */
void RemoveArgument(char **argument_array, int len);

void ClearString(char *string, int len);

#endif  // MAIN_3_H