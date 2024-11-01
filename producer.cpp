// Alicia Brown
// Assignment 1: Producer File
// Dr. Qiang Guan
// 11/1/24

#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/mman.h>
#include "shared.hpp"

int main(int argc, char* argv[]){
    
    int shmShared = shm_open(shmPath, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (shmShared == -1) {
        std::cerr << "Error creating shared memory: " << strerror(errno) << std::endl;
        return 1;
    }

    ftruncate(shmShared, sizeof(sharedData));

    sharedData *producer = static_cast<sharedData*>(mmap(nullptr, sizeof(sharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shmShared, 0));
    if (producer == MAP_FAILED) {
        std::cerr << "Error mapping shared memory: " << strerror(errno) << std::endl;
        close(shmShared);
        return 1;
    }

    // initalizing the semaphores, mutex, etc.
    producer->in = 0;
    producer->out = 0;

    // unlink semaphores
    sem_unlink("/empty_semaphore");
    sem_unlink("/full_semaphore");
    sem_unlink("/mutex_semaphore");
   

    // making sure that the semaphores do not become deprecated when running the programs
    producer->empty = sem_open("/empty_semaphore", O_CREAT, 0666, maxItems);
    producer->full = sem_open("/full_semaphore", O_CREAT, 0666, 0);
    producer->mutex = sem_open("/mutex_semaphore", O_CREAT, 0666, 1);


    if (producer->empty == SEM_FAILED || producer->full == SEM_FAILED) {
        std::cerr << "Error initializing semaphores: " << strerror(errno) << std::endl;
        munmap(producer, sizeof(sharedData));
        close(shmShared);
        return 1;
    }

    // how many items produced
    for (int i = 0; i < 2; ++i) {
        sleep(1);  // waiting to see if available to output
        sem_wait(producer->empty);
        sem_wait(producer->mutex); // locking the critical section
    

        producer->in = (producer->in + 1) % maxItems;  // reiterate until in ends
        std::cout << " " << std::endl;
        std::cout << "Produced : " << i << std::endl;


        sem_post(producer->mutex); // unlocking the mutex
        sem_post(producer->full);  // incrementing value

    }

    // cleanup
    sem_unlink("/empty_semaphore");
    sem_unlink("/full_semaphore");
    sem_unlink("/mutex_semaphore");
    shm_unlink(shmPath);
    sem_close(producer->empty);
    sem_close(producer->full);
    sem_close(producer->mutex);

    munmap(producer, sizeof(sharedData));
    close(shmShared);

    return 0;
}


