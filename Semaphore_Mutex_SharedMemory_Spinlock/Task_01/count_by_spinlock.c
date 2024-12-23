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

pthread_spinlock_t spinlock;
long long counter = 0;



void* count_by_spinlock(void* arg) {
    (void)arg;
    for (long long i = 0; i < MAX_COUNT; i++) {
        pthread_spin_lock(&spinlock);
        counter++;
        pthread_spin_unlock(&spinlock);
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREAD];
    pthread_spin_init(&spinlock, PTHREAD_PROCESS_PRIVATE);

    for (int i = 0; i < NUM_THREAD; i++) {
        pthread_create(&threads[i], NULL, count_by_spinlock, NULL);
    }

    for (int i = 0; i < NUM_THREAD; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_spin_destroy(&spinlock);
    printf("Counter: %lld\n", counter);

    return 0;
}