#include <linux/module.h>               // Module macro and function
#include <linux/fs.h>                   // File operations 
#include <linux/uaccess.h>              // Transfer data between kernel and user
#include <linux/device.h>               // For class_create function and device_create function
#include <linux/cdev.h>                 // cdev structure
#include <linux/kernel.h>               // Kernel macro and function
#include <linux/of.h>                   // For device tree
#include <linux/platform_device.h>      // For platform device

#define CLASS_NAME  "my_class"
#define DEVICE_NAME "my_device"
#define DRIVER_NAME "my_device_driver"

static dev_t device_id;                   
static struct class *my_class;

static int i = 0;

// Store device properties 
struct my_device_properties {
    int size;
    const char *serial_number;
    const char *permission;
    struct cdev cdev;
};

// Function prototypes
static int device_open(struct inode *inode, struct file *file);
static int device_release(struct inode *inode, struct file *file);
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t length, loff_t *offset);
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t length, loff_t *offset);

// File operations
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .release = device_release,
    .read = device_read,
    .write = device_write,
};

// Probe
static int my_device_probe(struct platform_device *pdev)
{
    struct device_node *dev_node = pdev->dev.of_node;
    struct device *dev = &pdev->dev;

    struct my_device_properties *data;
    const char *serial_number, *perm;
    u32 size;

    data = devm_kzalloc(dev, sizeof(*data), GFP_KERNEL);
    if (!data){
        return -ENOMEM;
    }

    if (!dev_node){
        return -EINVAL;
    }

    dev_info(&pdev->dev, "Probing device: %s\n", dev_node->name);

    if (of_property_read_u32(dev_node, "size", &size)) {
        dev_err(&pdev->dev, "Failed to read size.\n");
    }
    else{
        dev_info(&pdev->dev, "Size: %u\n", size);
        data->size = size;
    }

    if (of_property_read_string(dev_node, "serial_number", &serial_number)) {
        dev_err(&pdev->dev, "Failed to read serial_number.\n");
    }
    else{
        dev_info(&pdev->dev, "Serial Number: %s\n", serial_number);
        data->serial_number = serial_number;
    }

    if (of_property_read_string(dev_node, "perm", &perm)) {
        dev_err(&pdev->dev, "Failed to read perm.\n");
    }
    else {
        dev_info(&pdev->dev, "Permissions: %s\n", perm);
        data->permission = perm;
    }

    // Allocate for cdev
    if (!cdev_alloc()) {
        printk(KERN_ERR "Failed to allocate cdev.\n");
        return -ENOMEM;
    }

    // Init cdev
    cdev_init(&data->cdev, &fops);
    if (cdev_add(&data->cdev, (dev_t)MKDEV(MAJOR(device_id), MINOR(device_id) + i), 1) < 0) {
        printk(KERN_ERR "Failed to add cdev.\n");
        return -1;
    }

    // Create device file
    if (device_create(my_class, NULL, (dev_t)MKDEV(MAJOR(device_id), MINOR(device_id) + i), NULL, dev_node->name) == NULL) {
        cdev_del(&data->cdev);
        dev_err(&pdev->dev, "Failed to create device: %s\n", dev_node->name);
        return -EINVAL;
    }

    pdev->dev.platform_data = data; 

    dev_info(dev, "Device %s init done.\n", dev_node->name);

    i++;
    return 0;
}

// Remove
static int my_device_remove(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;
    struct my_device_properties *data = dev->platform_data;

    device_destroy(my_class, (dev_t)MKDEV(MAJOR(device_id), MINOR(device_id) + i));
    cdev_del(&data->cdev);
    devm_kfree(dev, data);
    i--;

    dev_info(&pdev->dev, "Device removed.\n");
    return 0;
}

// To compare with competible
static const struct of_device_id my_device_of_match[] = {
    { .compatible = "my,device1" },
    { .compatible = "my,device2" },
    { .compatible = "my,device3" },
    { .compatible = "my,device4" },
    {}
};

// Platform driver
static struct platform_driver my_device_driver = {
    .probe = my_device_probe,
    .remove = my_device_remove,
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = my_device_of_match,
    },
};

// Init
static int __init my_device_init(void)
{
    int ret;

    // Allocate for device id  --- TIm hieu QUang
    ret = alloc_chrdev_region(&device_id, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        printk(KERN_INFO "Failed to allocate device ID.\n");
        return ret;
    }

    // Create class
    my_class = class_create(CLASS_NAME);
    if (IS_ERR(my_class)) {
        unregister_chrdev_region(device_id, 1);
        printk(KERN_INFO "Failed to create class.\n");
        return PTR_ERR(my_class);
    }


    platform_driver_register(&my_device_driver);

    printk(KERN_INFO "Driver init done.\n");
    return 0;
}

// Exit
static void __exit my_device_exit(void)
{
    class_destroy(my_class);
    unregister_chrdev_region(device_id, 1);
    platform_driver_unregister(&my_device_driver);

    printk(KERN_INFO "Driver exited successfully.\n");
}

// Open
static int device_open(struct inode *inode, struct file *file)
{
    struct my_device_properties *data;
    
    if(!inode || !file) {
        prr_err("Open: Invalid inode or file!");
        return -EINVAL;
    }

    data = container_of(inode->i_cdev, struct my_device_properties, cdev);
    file->private_data = data;
    return 0;
}

// Release
static int device_release(struct inode *inode, struct file *file) {
    if(!inode || !file) {
        prr_err("Open: Invalid inode or file!");
        return -EINVAL;
    }

    printk(KERN_INFO "Device closed\n");
    return 0;
}

// Read
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t length, loff_t *offset)
{
    struct my_device_properties *data = file->private_data;
    char buffer[256];
    int buf_len;

    if (!data) {
        return -EFAULT;
    }

    // Check permission
    if(strcmp(data->permission, "r") != 0 && strcmp(data->permission, "rw") != 0) {
        pr_err("Read not permitted.\n");
        return -EACCES;
    }   

    buf_len = snprintf(buffer, sizeof(buffer), "Serial: %s.\n Size: %d.\n Permission: %s\n",
                       data->serial_number, data->size, data->permission);

    if (*offset >= buf_len) {
        return 0;
    }

    if (length > buf_len - *offset) {
        length = buf_len - *offset;
    }

    // Copy data from kernel space to user space
    if (copy_to_user(user_buffer, buffer + *offset, length)) {
        return -EFAULT;
    }
    *offset += length;
    return buf_len;
}

// Write
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t length, loff_t *offset)
{
    char *k_buffer;

    // Check permission
    if(strcmp(data->permission, "w") != 0 && strcmp(data->permission, "rw") != 0) {
        pr_err("Write not permitted.\n");
        return -EACCES;
    }   

    k_buffer = kmalloc(length + 1, GFP_KERNEL);
    if (!k_buffer) {
        return -ENOMEM;
    }

    if (copy_from_user(k_buffer, user_buffer, length)) {
        kfree(k_buffer);
        return -EFAULT;
    }

    k_buffer[length] = '\0';
    pr_info("Data: %s\n", k_buffer);
    kfree(k_buffer);
    return length;
}

module_init(my_device_init);
module_exit(my_device_exit);

// module_platform_driver(my_device_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ChinhPC2");
MODULE_DESCRIPTION("Test device tree");
