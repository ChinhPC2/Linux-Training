/******************************************************************************
 * 
 * ChinhPC2 
 * Task 1 - Unit 8
 * 
******************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREAD 4
#define MAX_COUNT 250000000
#define NUM_OFF_KEY 1

sem_t sem;
long long counter = 0;

void* count_by_semaphore(void* arg) {
    (void)arg;
    for (long long i = 0; i < MAX_COUNT; i++) {
        sem_wait(&sem);
        counter++;
        sem_post(&sem);
    }
    return NULL;
}

int main () {
    pthread_t threads[NUM_THREAD];
    sem_init(&sem, 0, NUM_OFF_KEY);

    for (int i = 0; i < NUM_THREAD; i++) {
        pthread_create(&threads[i], NULL, count_by_semaphore, NULL);
    }

    for (int i = 0; i < NUM_THREAD; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&sem);
    printf("Counter: %lld\n", counter);

    return 0;
}