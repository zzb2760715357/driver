#include <linux/init.h>
#include <linux/module.h>
#include <linux/notifier.h>


extern 	int test_notifier_call_chain(unsigned long val,void * v);

static int __init call_notifier(void)
{
	int err;

	printk("--------%s---------\r\n",__func__);
	printk("Begin to notify:\r\n");
	printk("==============================\r\n");
	err = test_notifier_call_chain(1,NULL);
	printk("==============================\r\n");
	if(err)
		printk("error test_notifier_call_chain\r\n");

	return err;
}


static void __exit uncall_notifier(void)
{
	printk("--------%s---------\r\n",__func__);
	printk("End notify\r\n");
}

MODULE_LICENSE("GPL");
module_init(call_notifier);
module_exit(uncall_notifier);








