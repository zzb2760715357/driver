#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>


unsigned int major = 250;

const char * name = "zbzhuang";

const struct file_operations test_fops;


static int __init test_init(void)
{
	int ret;

	printk("-------%s--------\r\n",__func__);
	printk("hello world\r\n");

	ret = register_chrdev(major,name,&test_fops);
	if(ret == 0){
		printk("Success register_chrdev\r\n");
	}else if(ret > 0){
		printk("Attemp to register_chrdev\r\n");
	}else{
		printk("Error to register_chrdev\r\n");
	}

	return 0;
}


static void __exit test_exit(void)
{
	unregister_chrdev(major,name);

	printk("-------%s--------\r\n",__func__);
}



module_init(test_init);
module_exit(test_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("zbzhuang");
MODULE_VERSION("V1.0");











