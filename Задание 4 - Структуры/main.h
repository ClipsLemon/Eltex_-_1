#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>

#define FIELD_SIZE 10
#define LIST_SIZE 100

typedef struct abonent {
  char name[FIELD_SIZE];
  char second_name[FIELD_SIZE];
  char tel[FIELD_SIZE];
} abonent;

void add_abonent(abonent *abonent_list);
void delete_abonent(abonent *abonent_list);
void search_by_name(abonent *abonent_list);
void show_all(abonent *abonent_list);

/**
 * @brief Функция обработки ввода. Нужна, для упрощения обработки ввода.
 * Записывает только целые положительные числа.
 * @param number - адрес на переменную, в которую будет осуществляться запись
 */
void input_number_handler(int *number);

/**
 * @brief Функция очистки буфера. Можно использовать вместо самописной функции
 * fflush, однако он считается не безопасным из-за неполной поддержки различными
 * компиляторами и возможности потери данных.
 */
void clear_input_buffer();

/**
 * @brief Функция выводит инферфейс и осуществляет навигацию по нему
 */
void print_interface(abonent *abonent_list);

/**
 * @brief Функция задает дефолтные значения, полностью заставляет поля \0
 *
 * @param object_field - поле объекта
 */
void set_default_data(char *object_field);

/**
 * @brief Опциональная функция, проводит инициализацию всего списка \0
 * значениями.
 *
 * @param abonent_list - список абонентов
 */
void initialization_list(abonent *abonent_list);

/**
 * @brief Функция для поиска ближайшего пустого абонента.
 *  Функция проходит по списку и проверяет, является ли объект пустым при помощи
 * функции is_empty.
 * @param abonent_list - список абонентов
 * @return -1 - дефолтное значение, пустных абонентов нет, все записано. Любое
 * другое значение, кроме -1 - был найден абонент под таким индексом.
 */
int find_empty(const abonent *abonent_list);

/**
 * @brief Функция проверяет поля абонента на пустоту. Достаточно проверки на \0
 * первого символа, так как все абоненты при инициализации и удалении
 * записываются как \0.
 *
 * @param abonent объект списка абонентов
 * @return 0 - абонент не пустой. 1 - абонент пустой.
 */
int is_empty(const abonent *abonent);

/**
 * @brief Функция выводит подсказки и делает запись данных в абонента.
 *
 * @param writeble_abonent - записываемый абонент
 */
void write_abonent(abonent *writeble_abonent);

/**
 * @brief Функция взятия строки. Используется метод fgets. Он записывает, в
 * некоторых случаях, в том числе \n. В функци проводится дополнительная
 * проверка на наличие этого символа в записанной строке. Если такого символа
 * нет, значит он повис в буфере и его надо очистить.
 *
 * @param string - адрес на строку, в которую осуществляется запись
 */
void get_string(char *string);

/**
 * @brief Функция возвращающая длину строки. Ищет до терминирующего нуля и
 * считает одновременно количество символов. Можно было бы использовать
 * стандартную strlen, но ради нее нужно подключать целую string.h, что в рамках
 * программы нецелесообразно.
 *
 * @param str - строка
 * @return size_t - длина строки
 */
size_t get_str_len(const char *str);

/**
 * @brief Функция осуществляет посимвольное сравнение строк.
 *
 * @param str1
 * @param str2
 * @return 0 - строки не равны, 1 - строки равны.
 */
int string_compare(char *str1, char *str2);

#endif