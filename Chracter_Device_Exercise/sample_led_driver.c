#include <linux/module.h>       // Kernel macro and function
#include <linux/fs.h>           // File operations 
#include <linux/uaccess.h>      // Transfer data between kernel and user
#include <linux/device.h>       // For class_create function and device_create function
#include <linux/cdev.h>         // cdev structure
#include <linux/gpio.h>         // GPIO

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ChinhPC2");
MODULE_DESCRIPTION("A sample led device driver");

// Define LED GPIO
#define LED_GPIO 529 // GPIO 17 (+ offset 512 on Raspberry Pi 3B)

#define DEVICE_NAME "led_device"
#define CLASS_NAME "led_class"

static dev_t device_id;                
static struct cdev *my_cdev;          
static struct class *my_class;        

static int led_stage = 0;

// Function prototypes
static int device_open(struct inode *inode, struct file *file);
static int device_close(struct inode *inode, struct file *file);
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t length, loff_t *offset);
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t key_len, loff_t *offset);

// File operations
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .release = device_close,
    .read = device_read,
    .write = device_write,
};

// Init
static int __init led_init(void) {
    int ret;

    // Request GPIO for LED
    ret = gpio_request(LED_GPIO, "LED_GPIO");
    if(ret != 0) {
	    printk(KERN_INFO "Request failed %d", ret);
	    return -1;
    }

    // Set the GPIO direction to output
    ret = gpio_direction_output(LED_GPIO, 0); // 0 is begin stage
    if(ret) {
        printk(KERN_INFO "Set GPIO failed.\n");
        return -1;
    }

    // Major and minor
    ret = alloc_chrdev_region(&device_id, 0, 1, DEVICE_NAME);
    if(ret < 0) {
        printk(KERN_INFO "Failed to alloc device ID.\n");
        return -1;
    }

    my_cdev = cdev_alloc();
    if (!my_cdev) {
        unregister_chrdev_region(device_id, 1);
        printk(KERN_ERR "Failed to allocate cdev.\n");
        return -ENOMEM;
    }

    // Init cdev
    cdev_init(my_cdev, &fops);
    if(cdev_add(my_cdev, device_id, 1) < 0) {
        unregister_chrdev_region(device_id, 1);
        printk(KERN_ERR "Failed to add cdev.\n");
        return -1;
    }

    // Create class
    my_class = class_create(CLASS_NAME);
    if(IS_ERR(my_class)) {
        cdev_del(my_cdev);
        unregister_chrdev_region(device_id, 1);
        printk(KERN_INFO "Failed to create class.\n");
        return PTR_ERR(my_class);
    }

    // Create device
    if(device_create(my_class, NULL, device_id, NULL, DEVICE_NAME) == NULL) {
        class_destroy(my_class);
        cdev_del(my_cdev);
        unregister_chrdev_region(device_id, 1);
        printk(KERN_INFO "Failed to create device.\n");
        return -1;
    }

    printk(KERN_INFO "Sample led init: /dev/%s\n", DEVICE_NAME);
    return 0;
}

// Exit
static void __exit led_exit(void) {
    device_destroy(my_class, device_id);
    class_destroy(my_class);
    cdev_del(my_cdev);
    unregister_chrdev_region(device_id,1);
    printk(KERN_INFO "Sample led device unregistered.\n");
}

module_init(led_init);
module_exit(led_exit);

// Open
static int device_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Device opened\n");
    return 0;
}

// Close
static int device_close(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Device closed\n");
    return 0;
}

// Read
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t length, loff_t *offset) {
    char led_buffer[20];
    int ret;

    if(*offset != 0) {
    	return 0;
    }

    // Update the buffer based on the LED state
    if (led_stage == 1) {
        snprintf(led_buffer, sizeof(led_buffer), "LED is on: 1\n");
    } else {
        snprintf(led_buffer, sizeof(led_buffer), "LED is off: 0\n");
    }

    if (length < strlen(led_buffer)) {
        return 1;  
    }

    // Copy data from kernel space to user space
    ret = copy_to_user(user_buffer, led_buffer, strlen(led_buffer));
    if (ret) {
        return -EFAULT;
    }

    *offset = strlen(led_buffer);
    return strlen(led_buffer);
}

// Write
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t key_len, loff_t *offset) {
    char key_buffer[5] = {0};

    if(key_len > 2) {
        key_len = 1;
    }

    if(copy_from_user(key_buffer, user_buffer, key_len)) {
        return -EFAULT;
    }

    if(key_buffer[0] == '1') {
        gpio_set_value(LED_GPIO, 1);
        led_stage = 1;
        printk(KERN_INFO "%s: LED is ON.", DEVICE_NAME);
    } else if(key_buffer[0] == '0') {
        gpio_set_value(LED_GPIO, 0);
        led_stage = 0;
        printk(KERN_INFO "%s: LED is OFF.", DEVICE_NAME);
    } else {
        printk(KERN_INFO "Invalid command!");
        return -EINVAL;
    }

    return key_len;
}
