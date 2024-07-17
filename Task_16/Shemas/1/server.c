#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "../color.h"

#define LISTEN_BACKLOG 10
#define IP_ADDR "127.0.0.1"
#define SIZE_BUFF 100
#define MAX_PORT 65535
#define SERV_PORT 9524
#define MAX_THREAD 10

// структура хранит информацию о подключениях клиентов
typedef struct {
  int *cl_list_len;
  // fd
  int *client_list;
  // индекс текущего клиента
  int curr_client;
} ClientInf;

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief Функция закрывает сокет и после закрытия перевыделяет память в
 * inf->client_list[curr_client], чтобы удалить ненужный дескриптор
 *
 * @param inf - структура ClientInf хранящая в себе указатели на длину списка
 * дескрипторов клииентов, сам список и индекс текущего клиента
 */
void CloseSocket(ClientInf *inf) {
  printf(YELLOW "START OF SOCKET %d CLOSURE\n" END_COLOR,
         inf->client_list[inf->curr_client]);

  if (close(inf->client_list[inf->curr_client]) == -1) {
    printf(RED "CLOSE ERROR %d: %s\n" END_COLOR,
           inf->client_list[inf->curr_client], strerror(errno));
  } else {
    int *copy_client_list = calloc(*(inf->cl_list_len) - 1, sizeof(int));
    if (copy_client_list == NULL) {
      printf(RED "CLOSE SOCKET CALLOC ERROR NULL: %s\n" END_COLOR,
             strerror(errno));
      printf(RED "SIZE = %d\n" END_COLOR, *inf->cl_list_len - 1);
    }
    // создаем копию массива дескрипторов на один меньше
    // копируем все до удаляемого дескриптора
    for (int i = 0; i < inf->curr_client; i++) {
      copy_client_list[i] = inf->client_list[i];
    }
    // копируем все после удаляемого дескриптора
    for (int i = inf->curr_client + 1; i < *(inf->cl_list_len); i++) {
      copy_client_list[i - 1] = inf->client_list[i];
    }

    *(inf->cl_list_len) -= 1;
    // удаляем старую память
    // free(inf->client_list);
    // и меняем значение указателя
    inf->client_list = copy_client_list;
  }

  printf(YELLOW "END OF SOCKET %d CLOSURE\n" END_COLOR,
         inf->client_list[inf->curr_client]);
}

void CopyList(int *list, int *copy_list, int len) {
  for (int i = 0; i < len; i++) {
    copy_list[i] = list[i];
  }
}

/**
 * @brief Поток ожидает от пользователя ключевого слова time. В ответ отправляет
 * время и завершает с ним работу.
 * После завершения работы с клиентом функция меняет информацию в client_list.
 *
 * @return void*
 */
void *AnswerClient(void *arg) {
  ClientInf *inf = (ClientInf *)arg;
  char buff[SIZE_BUFF];
  // слушаем клиента
  if (recv(inf->client_list[inf->curr_client], buff, SIZE_BUFF, 0) == -1) {
    printf(RED "RECEIVE ERROR: %s\n" END_COLOR, strerror(errno));
  }
  // клиент сможет отправить только "time", не обрабатываем информацию, а сразу
  // готовим ответ

  time_t rawtime;
  struct tm *timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);

  // Буфер для хранения строки с датой и временем

  // Формируем строку в формате YYYY.MM.DD | HH.MM.SS
  strftime(buff, sizeof(buff), "%Y.%m.%d | %H:%M:%S", timeinfo);

  if (send(inf->client_list[inf->curr_client], buff, SIZE_BUFF, 0) == -1) {
    printf(RED "SEND ERROR: %s\n" END_COLOR, strerror(errno));
  }
  // ждем сообщение о получении клиентом времени для закрытия сокета
  if (recv(inf->client_list[inf->curr_client], buff, SIZE_BUFF, 0) == -1) {
    printf(RED "RECEIVE ERROR: %s\n" END_COLOR, strerror(errno));
  }

  pthread_mutex_lock(&m1);
  CloseSocket(inf);
  pthread_mutex_unlock(&m1);

  return NULL;
}

/**
 * @brief прослушивает клиентов на подключение. В случае подключения создает
 * поток в котором пользователь запрашивает время у сервера
 *
 * @return * void
 */
void ListenClient(int listener_sfd, struct sockaddr_in *listener_addr,
                  socklen_t sock_len, int *cl_list_len, int *client_list) {
  printf(GREEN "LISTENER START\n" END_COLOR);
  pthread_t thread_list[MAX_THREAD];
  int curr_client = 0;
  *cl_list_len = 12;
  // в этот массив передается информацию о списке клиентов, длине списка и
  // текущем клиенте. используется для потоков.
  ClientInf inf_list[MAX_THREAD];
  int tmp_fd;
  client_list = calloc(*cl_list_len, sizeof(int));
  int *copy_list = NULL;
  // ждем подключения от клиента
  while (1) {
    tmp_fd = accept(listener_sfd, (struct sockaddr *)listener_addr, &sock_len);
    printf(BLUE "CLIENT ACCEPTED: FD %d\n" END_COLOR, tmp_fd);

    pthread_mutex_lock(&m1);
    client_list[*cl_list_len - 1] = tmp_fd;
    /*
    Критическая секция.
    Возможно одновременное изменение client_list и cl_list_len в функции
    CloseSocket
    */
    curr_client = *cl_list_len - 1;

    (*cl_list_len)++;
    printf("NEXT SIZE: %d\n", *cl_list_len);
    // копируем все и перевыделяем память
    if (copy_list != NULL) {
      free(copy_list);
    }
    copy_list = calloc(*cl_list_len, sizeof(int));
    CopyList(client_list, copy_list, (*cl_list_len) - 1);
    client_list = copy_list;
    // client_list = (int *)realloc(client_list, *cl_list_len);

    if (client_list == NULL) {
      printf(RED "REALLOC ERROR\n" END_COLOR);
    }
    pthread_mutex_unlock(&m1);
    // готовим структуру для потока
    inf_list[curr_client].cl_list_len = cl_list_len;
    inf_list[curr_client].client_list = client_list;
    inf_list[curr_client].curr_client = curr_client;

    // создаем поток под ответ для клиента
    pthread_create(&thread_list[curr_client], NULL, AnswerClient,
                   &(inf_list[curr_client]));
  }

  printf(GREEN "LISTENER END\n" END_COLOR);
}

int main() {

  int listener_sfd, cl_list_len, ip_addr, port;
  // список сокетов клиентов
  int *client_list;
  struct sockaddr_in listener_addr;

  int opt = 1;

  socklen_t sock_len = sizeof(listener_addr);

  cl_list_len = 1;
  listener_sfd = socket(AF_INET, SOCK_STREAM, 0);

  // Настраиваем сокет для повторного использования адреса и порта
  if (setsockopt(listener_sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    perror("setsockopt");
    close(listener_sfd);
    exit(EXIT_FAILURE);
  }

  if (listener_sfd == -1) {
    printf("SOCKET ERROR: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  memset(&listener_addr, 0, sizeof(listener_addr));

  listener_addr.sin_family = AF_INET;
  listener_addr.sin_port = htons(SERV_PORT);
  listener_addr.sin_addr.s_addr = ip_addr;

  while (1) {
    if (bind(listener_sfd, (struct sockaddr *)&listener_addr,
             sizeof(listener_addr)) == -1) {
      printf(RED "BIND ERROR: %s\n" END_COLOR, strerror(errno));
      // возможно была ранее сервер некорректно завершил работу, ребинд
      close(listener_sfd);
      sleep(1);
    } else {
      break;
    }
  }

  if (listen(listener_sfd, LISTEN_BACKLOG) == -1) {
    printf("LISTEN ERROR: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  // начинаем слушать клиентов
  ListenClient(listener_sfd, &listener_addr, sock_len, &cl_list_len,
               client_list);

  return 0;
}