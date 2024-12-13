/******************************************************************************
 * 
 * ChinhPC2 
 * Task 3 - Unit 5
 * 
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid, child_pid;

    int status;

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

        // Wait for the child process to finish
        pid_t wpid = waitpid(child_pid, &status, 0);
        if (wpid == -1) {
            perror("waitpid failed");
            exit(EXIT_FAILURE);
        }

        // Check the ending state of the child process
        if (WIFEXITED(status)) {
            printf("Child process ended normally with exit code %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Child process was terminated by signal %d\n", WTERMSIG(status));
        } else {
            printf("Child process ended abnormally\n");
        }
    }

    return 0;    
}