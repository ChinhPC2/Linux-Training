/******************************************************************************
 * 
 * ChinhPC2 
 * Task 1 - Unit 2
 * 
******************************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ChinhPC2");
MODULE_DESCRIPTION("Kernel Module A exporting variable x");

int x = 1012;
EXPORT_SYMBOL(x);

static int module_a_init(void) {
    printk(KERN_INFO "Module A loaded. Exported variable x = %d\n", x);
    return 0;
}

static void module_a_exit(void) {
    printk(KERN_INFO "Module A unloaded\n");
}

module_init(module_a_init);
module_exit(module_a_exit);
