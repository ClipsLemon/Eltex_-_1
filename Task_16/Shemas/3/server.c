#include <arpa/inet.h>
#include <errno.h>
#include <mqueue.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "../color.h"

#define LISTEN_BACKLOG 4
#define IP_ADDR "127.0.0.1"
#define SIZE_BUFF 100
#define SERV_PORT 9524
#define MAX_THREAD 100

#define DEFAULT_OFLGAS (O_CREAT | O_RDWR)
#define DEFAULT_MODE (S_IWUSR | S_IRUSR)

#define MESSAGE_LEN 4
#define MAX_MESSAGE MAX_THREAD
#define QUEUE_NAME "/accepted_client"

int shtdwn = 1;
mqd_t mqdes_client_queue;

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;


void *ThreadExit(void *arg) {
  printf(GREEN "EXIT THREAD HAS BEEN CREATED\n" END_COLOR);
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  char ext[6];

  struct sockaddr_in tmp;
  inet_pton(AF_INET, IP_ADDR, &tmp.sin_addr.s_addr);
  tmp.sin_family = AF_INET;
  tmp.sin_port = htons(SERV_PORT);

  memset(ext, 0, 6);

  while (strncmp(ext, "exit", 5) != 0) {
    fgets(ext, 5, stdin);
  }
  // если пользователь сервера вводит exit, то коннектимся к самому себе что бы
  // сбить accept в ListenClient и ставит shtdwn в 0
  shtdwn = 0;

  if (connect(sock, (struct sockaddr *)&tmp, sizeof(tmp)) == -1) {
    printf(RED "CONNECT ERROR: %s" END_COLOR, strerror(errno));
  }
  printf(GREEN "EXIT THREAD HAS BEEN CLOSED\n" END_COLOR);
}

/**
 * @brief Поток ожидает от пользователя ключевого слова time. В ответ отправляет
 * время и завершает с ним работу.
 * После завершения работы с клиентом функция меняет информацию в client_list.
 *
 * @return void*
 */
void *AnswerClient() {
  int client_fd;
  char buff[SIZE_BUFF];
  int priority = 0;
  while (shtdwn) {
    // ловим сообщение в очереди
    while (1) {
      if (mq_receive(mqdes_client_queue, (char *)&client_fd, MESSAGE_LEN,
                     &priority) == -1) {
        printf(RED "RECEIVE MESSAGE ERROR: %s\n" END_COLOR, strerror(errno));
        // чтобы не было спама ошибкой
        sleep(1);
      } else {
        break;
      }
    }
    // если -1, то сервер завершает работу
    if (client_fd == -1) {
      break;
    }
    // слушаем клиента
    if (recv(client_fd, buff, SIZE_BUFF, 0) == -1) {
      printf(RED "RECEIVE ERROR: %s\n" END_COLOR, strerror(errno));
    }
    printf(GREEN "GET MESSAGE FROM CLIENT %d\n" END_COLOR, client_fd);
    // клиент сможет отправить только "time", не обрабатываем информацию, а
    // сразу готовим ответ

    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // Формируем строку в формате YYYY.MM.DD | HH.MM.SS
    strftime(buff, sizeof(buff), "%Y.%m.%d | %H:%M:%S", timeinfo);

    if (send(client_fd, buff, SIZE_BUFF, 0) == -1) {
      printf(RED "SEND ERROR: %s\n" END_COLOR, strerror(errno));
    }
    printf(GREEN "SEND MESSAGE TO CLIENT %d\n" END_COLOR, client_fd);

    // ждем сообщение о получении клиентом времени для закрытия сокета
    if (recv(client_fd, buff, SIZE_BUFF, 0) == -1) {
      printf(RED "RECEIVE ERROR: %s\n" END_COLOR, strerror(errno));
    }

    if (close(client_fd) == -1) {
      printf(RED "CLOSE CLIENT SOCKET ERROR %d: %s\n" END_COLOR, client_fd,
             strerror(errno));
    }
    printf(GREEN "CLOSE CLIENT SOCKET %d\n" END_COLOR, client_fd);
  }
  printf(GREEN "THREAD HAS BEEN CLOSED\n" END_COLOR);

  return NULL;
}

/**
 * @brief Функция в любом случае отлинкует очередь и попытается закрыть
 * дескриптор очереди
 *
 * @param queue_id - дескриптотр
 * @param queue_name - имя очереди
 */
void QueueClose(mqd_t queue_id, char *queue_name) {
  int is_closed = 0;
  while (1) {
    // либо мы закрываем первы раз, либо не смогли отлинковать и отлинковываем
    // повторно
    if (is_closed || (mq_close(queue_id) == 0)) {
      is_closed = 1;
      if (mq_unlink(queue_name) == 0) {
        break;
      } else {
        printf(RED "ERROR: can\'t UNLINK queue: %s\n" END_COLOR, queue_name);
        break;
      }
    } else {
      printf(RED "ERROR: can\'t CLOSE queue: %s\n" END_COLOR, queue_name);
      sleep(2);
    }
  }

  printf(GREEN "LOG: A queue %s has been CLOSED successfully\n" END_COLOR,
         queue_name);
}

/**
 * @brief прослушивает клиентов на подключение. В случае подключения создает
 * поток в котором пользователь запрашивает время у сервера
 *
 * @return * void
 */
void ListenClient(int listener_sfd, struct sockaddr_in *listener_addr,
                  socklen_t sock_len) {
  printf(GREEN "LISTENER START\n" END_COLOR);
  int tmp_fd;

  pthread_t thread_list[MAX_THREAD];
  pthread_t thread_exit;

  pthread_create(&thread_exit, NULL, ThreadExit, NULL);
  // создаем потоки
  for (int i = 0; i < MAX_THREAD; i++) {
    pthread_create(&(thread_list[i]), NULL, AnswerClient, NULL);
    printf(GREEN "THREAD %d/%d IS CREATED\n" END_COLOR, i + 1, MAX_THREAD);
  }

  while (shtdwn) {
    tmp_fd = accept(listener_sfd, (struct sockaddr *)listener_addr, &sock_len);
    // отправляем дескриптор
    if (mq_send(mqdes_client_queue, (char *)&tmp_fd, MESSAGE_LEN, 1) == -1) {
      printf(RED "SEND ERROR: %s\n" END_COLOR, strerror(errno));
    }
  }
  tmp_fd = -1;

  // отправляем во все потоки -1 - знак закрытия
  for (int i = 0; i < MAX_THREAD; i++) {
    mq_send(mqdes_client_queue, (char *)&tmp_fd, MESSAGE_LEN, 1);
  }
  // ждем завершения всех потоков
  printf(GREEN "LISTENER END\n" END_COLOR);
}

int main() {
  int listener_sfd, ip_addr, port;
  // список сокетов клиентов
  struct sockaddr_in listener_addr;

  int opt = 1;
  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = MAX_MESSAGE;
  attr.mq_msgsize = MESSAGE_LEN;
  attr.mq_curmsgs = 0;

  while (1) {
    if ((mqdes_client_queue =
             mq_open(QUEUE_NAME, DEFAULT_OFLGAS, DEFAULT_MODE, &attr)) == -1) {
      printf(RED "ERROR: can\'t CREATE queue: %s\n" END_COLOR, QUEUE_NAME);
    } else {
      printf(GREEN "LOG: A queue %s has been CREATED successfully\n" END_COLOR,
             QUEUE_NAME);
      break;
    }
  }

  socklen_t sock_len = sizeof(listener_addr);
  inet_pton(AF_INET, IP_ADDR, &ip_addr);

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
  ListenClient(listener_sfd, &listener_addr, sock_len);

  // по завершению работы закрываем дескриптор и отлинковываем очередь
  QueueClose(mqdes_client_queue, QUEUE_NAME);

  return 0;
}