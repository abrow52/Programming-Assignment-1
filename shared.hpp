// Alicia Brown
// Assignment 1 - Header File
// Dr. Qiang Guan
// 11/1/24

#ifndef shared_h
#define shared_h

#include <pthread.h>
#include <semaphore.h>

#define maxItems 2

// shared variables
struct sharedData{
    sem_t *empty;
    sem_t *full;
    sem_t *mutex;
    int table[maxItems];
    int in, out;
};

const char* shmPath = "/unique_Memory";




#endif 
