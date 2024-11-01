// Alicia Brown
// Assignment 1: Consumer File
// Dr. Qiang Guan
// 11/1/24

#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/mman.h>
#include "shared.hpp"

int main(int argc, char *argv[]) {
    sleep(1); // make sure the producer runs first

    int shmShared = shm_open(shmPath, O_RDWR, S_IRUSR | S_IWUSR);
    if(shmShared == -1) {
        std::cerr << "Waiting for shared memory to be created..." << std::endl;
        sleep(1); // wait before retrying
    }

    ftruncate(shmShared, sizeof(int));

    sharedData *consumer = (sharedData*)(mmap(nullptr, sizeof(sharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shmShared, 0));
    if (consumer == MAP_FAILED) {
        std::cerr << "ERROR IN MAPPING SHARED MEMORY: " << strerror(errno) << std::endl;
        close(shmShared);
        return 1;
    }

    // initializing semaphores + variables for buffer.
    consumer->empty = sem_open("/empty_semaphore", 0);
    consumer->full = sem_open("/full_semaphore", 0);

    if (consumer->empty == SEM_FAILED || consumer->full == SEM_FAILED) {
        std::cerr << "Error opening semaphores: " << strerror(errno) << std::endl;
        munmap(consumer, sizeof(sharedData));
        close(shmShared);
        return 1;
    }

    // number of items consumed
    for (int i = 0; i < 2; ++i) {
        sleep(1);
        sem_wait(consumer->full);
        sem_wait(consumer->mutex);  // locking crit. section

        std::cout << " " << std::endl;
        std::cout << "Consumed : " << i << std::endl;

        consumer->out = (consumer->out + 1) % maxItems;  // keep reiterating

        sem_post(consumer->empty); // incrementing value
        sem_post(consumer->mutex); // leaves crit. section open
    }

    if (munmap(consumer, sizeof(sharedData)) == -1) {
         std::cerr << "Error unmapping shared memory: " << strerror(errno) << std::endl;
    }

    // cleanup
    close(shmShared);
    shm_unlink(shmPath);
    return 0;
}

