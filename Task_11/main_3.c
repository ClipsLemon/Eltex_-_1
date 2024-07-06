#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// ЦВЕТА

#define GREEN "\033[32m"
#define BLUE "\033[36m"
#define YELLOW "\033[33m"

#define END_COLOR "\033[0m"

// END ЦВЕТА

// количество магазинов
#define SHOP_NUMBER 5
// предельная вместимость магазина при старте
#define SHOP_BASE_CAPACITY 10000
// потребность покупателей
#define NEED_CUSTOMERS 100000
// вместимость грузчика
#define LOADER_CAPACITY 5000
// количество покупателей
#define CUSTOMERS_NUMBER 3
// магазин занят
#define BUSY 1
// магазин свободен
#define FREE 0

struct Shop {
  int products;
  int is_busy;
};

struct Customer {
  pthread_t thread_id;
  int customer_need;
  int customer_index;
};

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
struct Shop shop_list[SHOP_NUMBER];
int count_of_satisfied_customers = 0;

void CustomerReport(struct Customer *customer, int products_quantity,
                    int shop_number) {
  printf(BLUE "____________________________\n");
  printf("Я покупатель:    %d\n", customer->customer_index);
  printf("Зашел в магазин: %d\n", shop_number);
  printf("В нем есть:      %d\n", products_quantity);
  printf("Мне нужно:       %d\n", customer->customer_need);
  printf("____________________________\n" END_COLOR);
}

void LoaderReport(int products_quantity, int shop_number) {
  printf(YELLOW ".................................\n");
  printf("Я погрузчик\n");
  printf("Зашел в магазин: %d\n", shop_number);
  printf("В нем есть:      %d\n", products_quantity);
  printf("Стало столько:   %d\n", products_quantity + LOADER_CAPACITY);
  printf(".................................\n" END_COLOR);
}

void *ThreadLoader() {
  while (1) {
    // ищем свободный магазин с продуктами
    for (int i = 0; i < SHOP_NUMBER; i++) {
      if (shop_list[i].is_busy == FREE) {
        if (count_of_satisfied_customers == 3) {
          pthread_exit(NULL);
        }
        pthread_mutex_lock(&m1);
        LoaderReport(shop_list[i].products, i);
        shop_list[i].is_busy = BUSY;
        shop_list[i].products += LOADER_CAPACITY;
        shop_list[i].is_busy = FREE;
        pthread_mutex_unlock(&m1);
        // наелся и спит
        sleep(1);
      }
    }
  }
}

void *ThreadCustomer(void *customer) {
  struct Customer *customer_ptr = (struct Customer *)customer;
  customer_ptr->customer_need = rand() % NEED_CUSTOMERS + 1;
  int tmp_need = 0;

  int diffrence = 0;
  // пока клиент голоден
  while (customer_ptr->customer_need > 0) {
    // ищем свободный магазин с продуктами
    for (int i = 0; i < SHOP_NUMBER && customer_ptr->customer_need != 0; i++) {
      if (shop_list[i].is_busy == FREE && shop_list[i].products != 0) {
        pthread_mutex_lock(&m1);
        shop_list[i].is_busy = BUSY;
        tmp_need = customer_ptr->customer_need;
        CustomerReport(customer_ptr, shop_list[i].products, i);
        if (shop_list[i].products > customer_ptr->customer_need) {
          customer_ptr->customer_need = 0;
          shop_list[i].products -= tmp_need;
          // если в магазине недостаточно продуктов, то берем что можем и у
          // магазина отнимает то, что есть
        } else {
          customer_ptr->customer_need -= shop_list[i].products;
          shop_list[i].products = 0;
        }
        shop_list[i].is_busy = FREE;
        pthread_mutex_unlock(&m1);
        // наелся и спит
        sleep(2);
      }
    }
  }
  count_of_satisfied_customers++;
  printf(GREEN "- - - - Покупатель %d насытился - - - -\n" END_COLOR,
         customer_ptr->customer_index);
}

int main(void) {
  srand(time(NULL));

  struct Customer customer_list[CUSTOMERS_NUMBER];

  pthread_t customer_threads[CUSTOMERS_NUMBER];
  pthread_t loader_thread;

  // заполняем магазины
  for (int i = 0; i < SHOP_NUMBER; i++) {
    shop_list[i].products = rand() % SHOP_BASE_CAPACITY + 1;
    shop_list[i].is_busy = FREE;
  }

  for (int i = 0; i < CUSTOMERS_NUMBER; i++) {
    customer_list[i].customer_index = i;
    pthread_create(&(customer_list[i].thread_id), NULL, ThreadCustomer,
                   &customer_list[i]);
  }
  pthread_create(&loader_thread, NULL, ThreadLoader, NULL);

  for (int i = 0; i < CUSTOMERS_NUMBER; i++) {
    pthread_join(customer_list[i].thread_id, NULL);
  }
  pthread_join(loader_thread, NULL);
  printf(GREEN "Все покупатели насытилсь!\n" END_COLOR);
  return 0;
}
