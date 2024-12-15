/******************************************************************************
 * 
 * ChinhPC2 
 * Task 1 - Unit 6
 * 
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

void print_signal_mask() {
    sigset_t signal_set;
    
    // Retrieve signal
    if (sigprocmask(SIG_BLOCK, NULL, &signal_set) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    // Print
    printf("Current signal mask:\n");

    for (int sig = 1; sig < NSIG; ++sig) {
        if (sigismember(&signal_set, sig)) {
            printf("Signal %d is blocked\n", sig);
        }
    }
}

int main() {
    sigset_t new_set;

    // Creat signal to test
    if (sigemptyset(&new_set) == -1) {
        perror("sigemptyset");
        exit(EXIT_FAILURE);
    }

    if (sigaddset(&new_set, SIGINT) == -1) { // Block ctrl C
        perror("sigaddset");
        exit(EXIT_FAILURE);
    }

    // Block the signals in the new set
    if (sigprocmask(SIG_BLOCK, &new_set, NULL) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    printf("Blocked Ctrl C for test.\n");

    // Print
    print_signal_mask();
    return 0;
}
