/*
* ChinhPC2 
* Task 2 - Unit 1
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
    int fd, err;
    const char *file_path = "./src/task_02_sample.txt";
    char buffer[1024];

    // Open
    fd = open(file_path, O_CREAT | O_RDWR | O_TRUNC);
    if (fd == -1) {
        printf("Error opening");
        return 1;
    }

    // Enter input
    printf("Enter string: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Error input.\n");
        return 1;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
    
    // Write
    err = write(fd, buffer, strlen(buffer));
    if (err == -1) {
        printf("Error writing");
        close(fd);
        return 1;
    }

    // Seek from beginning of file. 
    err = lseek(fd, 0, SEEK_SET);
    if (err == -1) {
        printf("Error seeking");
        close(fd);
        return 1;
    }

    // Read
    memset(buffer, '\0', sizeof(buffer));
    err = read(fd, buffer, sizeof(buffer) - 1);
    if (err == -1) {
        printf("Error reading file!");
        close(fd);
        return 1;
    }

    buffer[err] = '\0';

    printf("Content of the file: %s\n", buffer);

    // Close
    close(fd);
    return 0;
}
