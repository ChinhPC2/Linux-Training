#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>


typedef struct {
    char full_name[100];
    short born_year;
    char phone_number[15];
    char home_town[100];
} human;

void* thread_1_handler(void* arg) {
    printf("Thread 1 is running: Verifying information...\n");
    sleep(1);
    pthread_exit(NULL);
}

void* thread_2_handler(void* arg) {
    human person = {
        .full_name = "Chinh Pham Cong",
        .born_year = 2002,
        .phone_number = "0969895xxx",
        .home_town = "Hai Duong, Viet Nam"
    };

    printf("Thread 2 is running: \n");
    printf("Full name: %s\n", person.full_name);
    printf("Born year: %hi\n", person.born_year);
    printf("Phone number: %s\n", person.phone_number);
    printf("Home town: %s\n", person.home_town);

    pthread_exit(NULL);
    }

int main () {
    pthread_t thread_1, thread_2;
    int ret_1, ret_2;

    ret_1 = pthread_create(&thread_1, NULL, thread_1_handler, NULL);
    if(ret_1) {
        fprintf(stderr, "Error: %d\n", ret_1);
        return EXIT_FAILURE;
    }

    ret_2 = pthread_create(&thread_2, NULL, thread_2_handler, NULL);
    if(ret_2) {
        fprintf(stderr, "Error: %d\n", ret_2);
        return EXIT_FAILURE;
    }

    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);

    return 0;
}
