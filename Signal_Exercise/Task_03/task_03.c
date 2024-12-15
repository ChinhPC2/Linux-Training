/******************************************************************************
 * 
 * ChinhPC2 
 * Task 3 - Unit 6
 * 
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void block_ctrl_c_signal_handler(int sig) {
    printf("SIGINT blocked - pending.\n");
}

int main() {
    while(1){
        sigset_t new_set, pending_set;

        // Register
        if (signal(SIGINT, block_ctrl_c_signal_handler) == SIG_ERR) {
            perror("signal");
            exit(EXIT_FAILURE);
        }

        // Creat empty mash
        if (sigemptyset(&new_set) == -1) {
            perror("sigemptyset");
            exit(EXIT_FAILURE);
        }

        // Raise SIGINT to 1
        if (sigaddset(&new_set, SIGINT) == -1) {
            perror("sigaddset");
            exit(EXIT_FAILURE);
        }

        // Block SIGINT
        if (sigprocmask(SIG_BLOCK, &new_set, NULL) == -1) {
            perror("sigprocmask");
            exit(EXIT_FAILURE);
        }

        printf("Waiting SIGINT signal.\n");

        // Wait Ctrl C for 5 seconds
        sleep(5);

        // Check pending
        if (sigpending(&pending_set) == -1) {
            perror("sigpending");
            exit(EXIT_FAILURE);
        }

        // Check SIGINT is pending
        if (sigismember(&pending_set, SIGINT)) {
            printf("\nSIGINT is pending. Exit.\n");
            exit(EXIT_SUCCESS);
        } else {
            printf("\nSIGINT is not pending. Please press Ctrl C.\n");
        }
    }  
    return 0;
}
