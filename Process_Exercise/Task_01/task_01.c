#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return -1;
    }

    const char *filename = argv[1];
    
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return -1;  
    }

    close(fd);
    return 0;  
}
