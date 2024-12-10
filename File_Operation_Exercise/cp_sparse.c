#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 4096

int main (int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s < source_file> <destination_file>\n", argv[0]);
        return 1;
    }

    int src_fd = open(argv[1], O_RDONLY);
    if (src_fd < 0) {
        printf("Error opening source file.\n");
        return 1;
    }

    int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd < 0) {
        close(src_fd);
        printf("Error opening destination file.\n");
        return 1;
    }
    
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_write;
    off_t offset;

    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        size_t i = 0;
        while (i < bytes_read) {
        
            // Find start hole
            if (buffer[i] == '\0') {
                size_t hole_start = i;
                while (i < bytes_read && buffer[i] == '\0') {
                    i++;
                }
                
                // Find past hole in destination file
                offset = lseek(dest_fd, i - hole_start, SEEK_CUR);
                if (offset == -1) {
                    close(src_fd);
                    close(dest_fd);
                    printf("Error coppy to destination file.\n");
                    return 1;
                }
            } else {
                size_t data_start = i;
                while (i < bytes_read && buffer[i] != '\0') {
                    i++;
                }

                bytes_write = write(dest_fd, &buffer[data_start], i - data_start);
                if (bytes_write < 0) {
                    close(src_fd);
                    close(dest_fd); 
                    printf("Error writing to destination file.\n");  
                    return 1;                
                }
            }
        }
        
    }
    
    if(bytes_read < 0) {
        close(src_fd);
        close(dest_fd); 
        printf("Error reading source file.\n");                  
    }

    close(src_fd);
    close(dest_fd); 
    printf("Done\n");  

    return 0;
}

