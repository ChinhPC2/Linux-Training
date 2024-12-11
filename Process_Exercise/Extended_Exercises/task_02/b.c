#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = getpid();
    pid_t ppid = getppid();

    printf("Program B: PID = %d, PPID = %d\n", pid, ppid);
    return 0;
}