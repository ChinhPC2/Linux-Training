/******************************************************************************
 * 
 * ChinhPC2 
 * Task 4 - Unit 1
 * 
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    // Open
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        printf("Error opening file");
        return 1;
    }

    char buffer[1024];
    int err;

    // Read and display
    while ((err = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[err] = '\0';
        printf("%s", buffer);
    }

    if (err == -1) {
        printf("Error reading file");
        return 1;
    }

    // Close
    close(fd);
    return 0;
}

