//file for the consumer

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sempaphore.h>
#include <unistd.h>
#define maxItems

extern int table[maxItems]
extern int count;

sem_t empty;
sem_t full;
pthread_mutex_t mutex;

void* consumer(void* arg){
  for (int i = 0; i < 10; i++){
    sem_wait(&full);
    pthread_mutex_lock(&mutex);

    //consumer an item
    count--;
    print("Consumed: ", table[count]);

    pthread_mutex_unlock(&mutex);
    sem_post(&empty);

    sleep(rand() % 2);
  }
  return NULL;
}

int main(){
  pthread_t cons_thread;

  sem_init(&empty, 0, maxItems);
  sem_init(&full, 0, 0);
  pthread_mutex_init(&mutex, NULL);

  pthread_create(&cons_thread, NULL, consumer, NULL);
  pthread_join(cons_thread, NULL);

  sem_destroy(&empty);
  sem_destroy(&full);
  pthread_mutex_destroy(&mutex);
  return 0;
}


