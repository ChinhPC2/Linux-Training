/******************************************************************************
 * 
 * ChinhPC2 
 * Task 2 - Unit 5
 * 
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid, child_pid;

    pid = fork();

    if (pid < 0)
    {
        printf("Error fork.\n");
        return 1;
    } else if (pid == 0)
    {
        // Replace child process with program B
        execl("./b", "b", (char *)NULL);

        printf("Error execl.\n");
        return 1;
    } else {
        child_pid = pid;
        pid_t parent_pid = getpid();
        printf("Program A: PID = %d, Child PID = %d.\n", parent_pid, child_pid);

        wait(NULL);
    }
    
    return 0;    
}