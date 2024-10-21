//file for the producer
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define maxItems

int table[maxItems]
int count = 0;

sem_t empty;
sem_t full;
pthread_mutex_t mutex;

void* producer(void* arg){
  for (int i = 0; i < 10; i++){
    sem_wait(&empty);
    pthread_mutex_lock($mutex);

    //produce an item
    table("Produced: ", table[count]);
    count++;

    pthread_mutex_unlock(&mutex);
    sem_post(&full);

    sleep(rand() % 2);
  }
  return NULL;
}

int main(){
  pthread_t prod_thread;

  sem_init(&empty, 0, maxItems);
  sem_init(&full, 0, 0);
  pthread_mutex_init(&mutex, NULL);

  pthread_Create(&prod_thread, NULL, producer, NULL);
  pthread_join(prod_thread, NULL);

  sem_destroy(&empty);
  sem_destroy(&full);
  pthread_mutex_destroy(&mutex);
  return 0;
}


