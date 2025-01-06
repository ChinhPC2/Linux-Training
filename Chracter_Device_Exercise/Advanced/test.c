#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>

#define DEVICE_COUNT 3

#define LED 'L'
#define IOCTL_LED_ON _IO(LED, 1)
#define IOCTL_LED_OFF _IO(LED, 2)
#define IOCTL_LED_GET_stage _IOR(LED, 3, int)

void test_led_control(int fd, int minor) {
    int ret, stage;

    // Turn on
    ret = ioctl(fd, IOCTL_LED_ON, NULL);
    if (ret < 0) {
        perror("Failed to turn ON");
        return;
    }
    printf("LED %d turned ON.\n", minor);

    // Get stage
    ret = ioctl(fd, IOCTL_LED_GET_stage, &stage);
    if (ret < 0) {
        perror("Failed to get stage");
        return;
    }
    printf("LED %d: %d\n", minor, stage);

    // Turn LED OFF
    ret = ioctl(fd, IOCTL_LED_OFF, NULL);
    if (ret < 0) {
        perror("Failed to turn OFF");
        return;
    }
    printf("LED %d turned OFF.\n", minor);

    // Get stage
    ret = ioctl(fd, IOCTL_LED_GET_stage, &stage);
    if (ret < 0) {
        perror("Failed to get stage");
        return;
    }
    printf("LED %d: %d\n", minor, stage);
}

int main() {
    int fd[DEVICE_COUNT];
    char device_path[32];

    // Open LED devices
    int red_fd = open("/dev/LED_01", O_RDWR);
    if (red_fd < 0) {
        perror("Failed to open RED device");
        return EXIT_FAILURE;
    }

    int green_fd = open("/dev/LED_02", O_RDWR);
    if (green_fd < 0) {
        perror("Failed to open GREEN device");
        close(red_fd);
        return EXIT_FAILURE;
    }

    int blue_fd = open("/dev/LED_03", O_RDWR);
    if (blue_fd < 0) {
        perror("Failed to open BLUE device");
        close(red_fd);
        close(green_fd);
        return EXIT_FAILURE;
    }

    // Test LED control for each device
    for (int i = 0; i < DEVICE_COUNT; i++) {
        test_led_control(fd[i], i);
    }

    // Close device files
    for (int i = 0; i < DEVICE_COUNT; i++) {
        close(fd[i]);
    }

    return EXIT_SUCCESS;
}
