#include <stdio.h>
#include <signal.h>

void main () {
    
    sigset_t my_sig_set;
    sigemptyset(&my_sig_set);

    sigaddset(&my_sig_set, SIGINT);
    sigprocmask(SIG_BLOCK, &my_sig_set, NULL);

    while(1);
}