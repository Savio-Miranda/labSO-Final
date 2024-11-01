#ifndef GAMESEMAPHORE_H
#define GAMESEMAPHORE_H

#include <semaphore.h>

class GameSemaphore {
    public:
        GameSemaphore(int value) {
            sem_init(&sem, 0, value);
        }

    void destroy_semaphore() {
        sem_destroy(&sem);
    }

    void wait_semaphore() { // operação de espera
        sem_wait(&sem);
    }

    void release_semaphore() { // operação de sinal
        sem_post(&sem);
    }

private:
    sem_t sem;
};

#endif