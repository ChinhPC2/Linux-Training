/******************************************************************************
 * 
 * ChinhPC2 
 * Task 1 - Unit 6
 * 
******************************************************************************/

#include <stdio.h>
#include <signal.h>

void block_ctrl_c_signal_handler(int signo) {
    printf(" - Ctrl C blocked!\n");
}

void main() {
    signal(SIGINT, block_ctrl_c_signal_handler);
    while(1);
}
