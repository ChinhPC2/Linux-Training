/******************************************************************************
 * 
 * ChinhPC2 
 * Task 3 - Unit 1
 * 
******************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    int fd;
    char process_ID[10];
    char file_path[256];
    char buffer[100];
    int err;
    
    // Enter process ID
    printf("Enter the process ID: ");
    if (fgets(process_ID, sizeof(process_ID), stdin) == NULL) {
        printf("Error input\n");
        return 1;
    }

    // Remove "\n" symbol
    for (int i = 0; process_ID[i] != '\0'; i++) {
        if (process_ID[i] == '\n') {
            process_ID[i] = '\0';
            break;
        }
    }

    // Make file path
    snprintf(file_path, sizeof(file_path), "/proc/%s/comm", process_ID);

	// Open
    fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        printf("Error opening file");
        return 1;
    }

    // Read the process name from the file
    err = read(fd, buffer, sizeof(buffer) - 1);
    if (err == -1) {
        printf("Error reading file");
        close(fd);
        return 1;
    }
    buffer[err] = '\0';

    printf("Process: %s\n", buffer);

    close(fd);
    return 0;
}
