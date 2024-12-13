/******************************************************************************
 * 
 * ChinhPC2 
 * Task 5 - Unit 1
 * 
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

int main(void) {
    int fd;
    unsigned char data[3];
    int x = 0, y = 0;

    fd = open("/dev/input/mice", O_RDONLY);
    if (fd == -1) {
        perror("Error opening /dev/input/mice");
        return 1;
    }

    printf("Tracking mouse... Press Ctrl C to stop.");

    while (1) {
        // Read mouse data
        if (read(fd, data, sizeof(data)) != sizeof(data)) {
            perror("Error reading data\n");
            close(fd);
            return 1;
        }

        signed char delta_x = (signed char)data[1];
        signed char delta_y = (signed char)data[2];

        x += delta_x;
        y += delta_y;

        printf("Mouse coordinates: x=%d, y=%d\n", x, y);
    }

    close(fd);
    return 0;
}
