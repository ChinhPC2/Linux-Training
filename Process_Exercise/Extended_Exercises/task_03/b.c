/******************************************************************************
 * 
 * ChinhPC2 
 * Task 3 - Unit 5
 * 
******************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = getpid();
    pid_t ppid = getppid();

    printf("Program B: PID = %d, PPID = %d\n", pid, ppid);
    return 100;
}