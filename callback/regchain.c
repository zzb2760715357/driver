#include <linux/init.h>
#include <linux/module.h>
#include <linux/notifier.h>




extern int register_test_notifier(struct notifier_block * nb);

extern int unregister_test_notifier(struct notifier_block * nb);

static int test_event1(struct notifier_block * this, unsigned long event, void *ptr)
{
	printk("In Event1:Event Number is %ld\r\n",event);

	return 0;
}

static int test_event2(struct notifier_block * this, unsigned long event, void *ptr)
{
	printk("In Event2:Event Number is %ld\r\n",event);

	return 0;
}

static int test_event3(struct notifier_block * this, unsigned long event, void *ptr)
{
	printk("In Event3:Event Number is %ld\r\n",event);

	return 0;
}








static struct notifier_block test_notifier1 =
{
	.notifier_call = test_event1,
};

static struct notifier_block test_notifier2 =
{
	.notifier_call = test_event2,
};

static struct notifier_block test_notifier3 =
{
	.notifier_call = test_event3,
};




static int __init reg_notifier(void)
{
	int err;
	printk("----------%s-----------\r\n",__func__);


	printk("Begin to register:\n");

	err = register_test_notifier(&test_notifier1);
	if(err){
		printk("error register_test_notifier1\r\n");
		return -1;
	}
	printk("Success register test_notifier1\r\n");

	err = register_test_notifier(&test_notifier2);
	if(err){
		printk("error register_test_notifier2\r\n");
		return -1;
	}
	printk("Success register test_notifier2\r\n");

	err = register_test_notifier(&test_notifier3);
	if(err){
		printk("error register_test_notifier3\r\n");
		return -1;
	}
	printk("Success register test_notifier3\r\n");


	return err;
}


static void __exit unreg_notifier(void)
{
	printk("----------%s-----------\r\n",__func__);
	unregister_test_notifier(&test_notifier1);
	unregister_test_notifier(&test_notifier2);
	unregister_test_notifier(&test_notifier3);
}




module_init(reg_notifier);
module_exit(unreg_notifier);
MODULE_LICENSE("GPL");




















