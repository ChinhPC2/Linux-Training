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
MODULE_DESCRIPTION("Kernel Module B using variable x from Module A");

extern int x;

static int __init module_b_init(void) {
    printk(KERN_INFO "Module B loaded. Using variable x = %d\n", x);
    // printk(KERN_INFO "Module B loaded. Using variable x\n");
    return 0;
}

static void module_b_exit(void) {
    printk(KERN_INFO "Module B unloaded.\n");
}

module_init(module_b_init);
module_exit(module_b_exit);
