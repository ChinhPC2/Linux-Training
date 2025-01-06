#include <linux/module.h>       // Kernel macro and function
#include <linux/fs.h>           // File operations 
#include <linux/uaccess.h>      // Transfer data between kernel and user
#include <linux/device.h>       // For class_create function and device_create function
#include <linux/cdev.h>         // cdev structure
#include <linux/gpio.h>         // GPIO
#include <linux/ioctl.h>        // For ioctl

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ChinhPC2");
MODULE_DESCRIPTION("A multi-led device driver");

// Define LED GPIO (+ offset 512 on Raspberry Pi 3B)
#define LED_RED 529         // GPIO 17 
#define LED_GREEN 534       // GPIO 22
#define LED_BLUE 539        // GPIO 27

#define DEVICE_NAME	 "LED_devices"
#define CLASS_NAME	 "multi-led_class"
#define DEVICE_COUNT 3

#define LED 'L'
#define IOCTL_LED_ON _IO(LED, 1)
#define IOCTL_LED_OFF _IO(LED, 2)
#define IOCTL_LED_GET_STATE _IOR(LED, 3, int)

static dev_t device_id;                
static struct cdev *my_cdev[DEVICE_COUNT];          
static struct class *my_class;        

static int led[DEVICE_COUNT] = {LED_RED, LED_GREEN, LED_BLUE};
static int led_stage[DEVICE_COUNT] = {0};

// Function prototype
static long device_ioctl(struct file *file, unsigned int command, unsigned long arg);

// File operations
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = device_ioctl,
};

// Init
static int __init led_init(void) {
    int ret;

    for (int i = 0; i < DEVICE_COUNT; i ++) {
        // Request GPIO for LED
        ret = gpio_request(led[i], "LED_GPIO");
        if(ret != 0) {
            printk(KERN_ERR "Request failed %d", ret);
            return -1;
        }

        // Set the GPIO direction to output
        ret = gpio_direction_output(led[i], 0); // 0 is begin stage
        if(ret) {
            printk(KERN_ERR "Set GPIO failed.\n");
            return -1;
        }
    }

    // Major and minor
    ret = alloc_chrdev_region(&device_id, 0, DEVICE_COUNT, DEVICE_NAME);
    if(ret < 0) {
        printk(KERN_ERR "Failed to alloc device ID.\n");
        return -1;
    }

    // Create class
    my_class = class_create(CLASS_NAME);
    if(IS_ERR(my_class)) {
        unregister_chrdev_region(device_id, 1);
        printk(KERN_INFO "Failed to create class.\n");
        return PTR_ERR(my_class);
    }

    for (int i = 0; i < DEVICE_COUNT; i ++) {
        my_cdev[i] = cdev_alloc();
        if (!my_cdev[i]) {
            unregister_chrdev_region(device_id, DEVICE_COUNT);
            printk(KERN_ERR "Failed to allocate cdev.\n");
            return -ENOMEM;
        }

        // Init cdev
        cdev_init(my_cdev[i], &fops);
        if(cdev_add(my_cdev[i], MKDEV(MAJOR(device_id), MINOR(device_id) + i), 1) < 0) {
            unregister_chrdev_region(device_id, DEVICE_COUNT);
            printk(KERN_ERR "Failed to add cdev.\n");
            return -1;
        }

        // Create device
        char device_name[20];
        sprintf(device_name, "LED-0%d", i + 1);

        if(device_create(my_class, NULL, MKDEV(MAJOR(device_id), MINOR(device_id) + i), NULL, device_name) == NULL) {
            class_destroy(my_class);
            cdev_del(my_cdev[i]);
            unregister_chrdev_region(device_id, 1);
            printk(KERN_ERR "Failed to create device.\n");
            return -1;
        }
    }

    printk(KERN_INFO "Sample led init: /dev/%s\n", DEVICE_NAME);
    return 0;
}

// Exit
static void __exit led_exit(void) {
    for (int i = 0; i < DEVICE_COUNT; i++) {
        device_destroy(my_class, MKDEV(MAJOR(device_id), MINOR(device_id) + i));
        cdev_del(my_cdev[i]);
    }
    class_destroy(my_class);
    unregister_chrdev_region(device_id,1);
    printk(KERN_INFO "Sample led device unregistered.\n");

    for (int i = 0; i < DEVICE_COUNT; i++) {
        gpio_free(led[i]);
    }
}

module_init(led_init);
module_exit(led_exit);

// Input - Output control
static long device_ioctl(struct file *file, unsigned int command, unsigned long arg) {
    int minor = MINOR(file_inode(file)->i_rdev);
    int ret = 0;

    switch (command) {
        case IOCTL_LED_ON:
            gpio_set_value(led[minor], 1);
            led_stage[minor] = 1;
            printk(KERN_INFO "LED %d is turned ON.\n", minor);
            break;

        case IOCTL_LED_OFF:
            gpio_set_value(led[minor], 0);
            led_stage[minor] = 0;
            printk(KERN_INFO "LED %d is turned OFF.\n", minor);
            break;

        case IOCTL_LED_GET_STATE:
            ret = copy_to_user((int __user *)arg, &led_stage[minor], sizeof(int));
            if (ret) {
                printk(KERN_ERR "Failed to copy LED state to user.\n");
                return -EFAULT;
            }
            for (int i = 0; i < DEVICE_COUNT; i ++) {
                printk(KERN_INFO "LED %d: (%d).\n", minor, led_stage[i]);
            }
            break;
        
        default:
            printk(KERN_INFO "Invalid command.\n");
            return -EINVAL;
    }
    return 0;
}
