#include <linux/module.h>
#include <linux/init.h>
#include "../include/haha.h"

void haha(void)
{
	printk("haha!\r\n");
}

EXPORT_SYMBOL(haha);
MODULE_LICENSE("GPL");

