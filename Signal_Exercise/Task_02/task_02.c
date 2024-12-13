/******************************************************************************
 * 
 * ChinhPC2 
 * Task 2 - Unit 6
 * 
******************************************************************************/

#include <stdio.h>
#include <signal.h>
#include <string.h>

void print_signal_mask() {
    sigset_t my_sig_set;

    if (sigprocmask(0, NULL, &my_sig_set) == -1)    
    {
        perror("sigprocmask");
        return;
    }


    // Print result
    printf("Block signal:\n");
    for (int i = 1; i < NSIG; i++) {
        if (sigismember(&my_sig_set, i)) {
            printf("Blocked signal: %d - %s", i, strsignal(i));
        }
    }
}

int main () {
    print_signal_mask();
    return 0;
}