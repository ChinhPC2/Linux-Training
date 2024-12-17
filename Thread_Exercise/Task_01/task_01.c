/******************************************************************************
 * 
 * ChinhPC2 
 * Task 2 - Unit 7
 * 
******************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define START 1
#define END 1000000000
#define THREAD_COUNT 6

// Function to count without thread
long long count_odd_without_threads(long long start, long long end) {
    long long count = 0;
    
    for (long long i = start; i < end; i+=2)
    {
        count++;
    }
    return count;
}

typedef struct {
    long long start;
    long long end;
    long long count;
} thread_data;

// Function executed by each thread
void* each_thread_count(void *arg) {
    thread_data* data = (thread_data*)arg;
    data->count = 0;

    for (long long i = data->start; i <= data->end; i +=2) {
        data->count++;
    }
    return NULL;
}

// Function to count with thread
long long count_odd_with_threads(long long start, long long end) {
    pthread_t threads[THREAD_COUNT];
    thread_data thread_data[THREAD_COUNT];

    long long range = (end - start + 1) / THREAD_COUNT;

    // Each thread
    for (int i = 0; i < THREAD_COUNT; i++) {
        thread_data[i].start = start + i * range + (start % 2 == 0 ? 1 : 0);
        thread_data[i].end = (i == THREAD_COUNT - 1) ? end : start + (i + 1) * range - 1;
        pthread_create(&threads[i], NULL, each_thread_count, &thread_data[i]);
    }

    // Wait complete
    long long total_count = 0;
    for (int i = 0; i < THREAD_COUNT; i ++) {
        pthread_join(threads[i], NULL);
        total_count += thread_data[i].count;
    }
    return total_count;
}


int main() {
    clock_t start, end;
    double time_without_thread, time_with_thread;
    long long result_without_thread, result_with_thread;

    // Case 1: Without threading
    start = clock();
    result_without_thread = count_odd_without_threads(START, END);
    end = clock();
    time_without_thread = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Case 2: With threading
    start = clock();
    result_with_thread = count_odd_with_threads(START, END);
    end = clock();
    time_with_thread = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Print result
    printf("Without multithreading:\n");
    printf("Result: %lld\n", result_without_thread);
    printf("Time taken: %.2f seconds\n", time_without_thread);

    printf("\n-------------\n");

    printf("With multithreading:\n");
    printf("Result: %lld\n", result_with_thread);
    printf("Time taken: %.2f seconds\n", time_with_thread);

    return 0;
}