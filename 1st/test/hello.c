#include <linux/init.h>
#include <linux/module.h>




static int __init test_init(void)
{
	printk("-------%s--------\r\n",__func__);
	printk("hello world\r\n");

	return 0;
}


static void __exit test_exit(void)
{
	printk("-------%s--------\r\n",__func__);
}



module_init(test_init);
module_exit(test_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("zbzhuang");
MODULE_VERSION("V1.0");











