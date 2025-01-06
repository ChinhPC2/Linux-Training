#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/platform_device.h>

#define DRIVER_NAME "my_device_driver"

// Function to probe devices
static int my_device_probe(struct platform_device *pdev)
{
    struct device_node *np = pdev->dev.of_node;
    const char *serial_number, *perm;
    u32 size;

    if (!np)
        return -EINVAL;
    dev_info(&pdev->dev, "Probing device: %s\n", np->name);

    if (of_property_read_u32(np, "size", &size))
        dev_err(&pdev->dev, "Failed to read size.\n");
    else
        dev_info(&pdev->dev, "Size: %u\n", size);

    if (of_property_read_string(np, "serial_number", &serial_number))
        dev_err(&pdev->dev, "Failed to read serial_number.\n");
    else
        dev_info(&pdev->dev, "Serial Number: %s\n", serial_number);

    if (of_property_read_string(np, "perm", &perm))
        dev_err(&pdev->dev, "Failed to read perm.\n");
    else
        dev_info(&pdev->dev, "Permissions: %s\n", perm);

    return 0;
}

static int my_device_remove(struct platform_device *pdev) {
    dev_info(&pdev->dev, "Device removed.\n");
    return 0;
}

// Compatible ID matching
static const struct of_device_id my_device_of_match[] = {
    { .compatible = "my,device1" },
    { .compatible = "my,device2" },
    { .compatible = "my,device3" },
    { .compatible = "my,device4" },
    {}
};
//MODULE_DEVICE_TABLE(of, my_device_of_match);

static struct platform_driver my_device_driver = {
    .probe = my_device_probe,
    .remove = my_device_remove,
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = my_device_of_match,
    },
};

module_platform_driver(my_device_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ChinhPC2");
MODULE_DESCRIPTION("Test device tree");
