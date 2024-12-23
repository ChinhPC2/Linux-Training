/******************************************************************************
 * 
 * ChinhPC2 
 * Task 1 - Unit 8
 * 
******************************************************************************/

#include <stdio.h>
#include <pthread.h>

#define NUM_THREAD 4
#define MAX_COUNT 250000000

pthread_mutex_t mutex;
long long counter = 0;

void* count_by_mutex(void* arg) {
    (void)arg;
    for (long long i = 0; i < MAX_COUNT; i++) {
        pthread_mutex_lock(&mutex);
        counter++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main () {
    pthread_t threads[NUM_THREAD];
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < NUM_THREAD; i++) {
        pthread_create(&threads[i], NULL, count_by_mutex, NULL);
    }

    for (int i = 0; i < NUM_THREAD; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    printf("Counter: %lld\n", counter);

    return 0;
}
