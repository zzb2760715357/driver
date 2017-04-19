#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>


#define DEBUG_SWITCH 1
#if DEBUG_SWITCH 
#define P_DEBUG(fmt,args...) printk("<1>""[%s]"fmt,__func__,##args)
#else
#define P_DEBUG(fmt,args...) printk("<7>""[%s]"fmt,__func__,##args)
#endif


unsigned int major = 250;
unsigned int minor = 0;
dev_t devno;

struct cdev  cdev_test;


const char * name = "zbzhuang";




int test_open (struct inode * node, struct file * filp)
{
	printk("-------%s--------\r\n",__func__);

	return 0;
}
int test_close (struct inode * node, struct file * filp)
{
	printk("-------%s--------\r\n",__func__);

	return 0;
}

const struct file_operations test_fops = {
	.open = test_open,
	.release = test_close,
};


static int __init test_init(void)
{

	int ret = 0;

	printk("-------%s--------\r\n",__func__);
	printk("hello world\r\n");


	/*申请设备号*/
	if(major){
		devno = MKDEV(major,minor);	
		ret = register_chrdev_region(devno,1,"test new driver");//查看在哪里打印的啊
	}else{
		ret =alloc_chrdev_region(&devno,minor,1,"alloc chrdev no");
		major = MAJOR(devno);
		minor = MINOR(devno);
		
	}

	P_DEBUG("major[%d]\tminor[%d]\r\n",major,minor);

	if(ret < 0 ){
		P_DEBUG("error register device\r\n");
		//ret = -EBUSY;
		goto err0;
	}else{
		P_DEBUG("Success register device\r\n");
	}
	

	/* 设备的注册 */
	cdev_init(&cdev_test,&test_fops);
	cdev_test.owner = THIS_MODULE;

	/*
		将cdev 和 devno关联起来
	*/
	ret = cdev_add(&cdev_test,devno,1);
	if(ret < 0){
		P_DEBUG("error register device\r\n");
		//ret = -ENODEV;
		goto err1;
	}else{
		P_DEBUG("Success cdev add\r\n");
	}

	return 0;
	

err1:
	unregister_chrdev_region(devno,1);
err0:
	return ret;
}


static void __exit test_exit(void)
{

	cdev_del(&cdev_test);
	unregister_chrdev(major,name);

	printk("-------%s--------\r\n",__func__);
}



module_init(test_init);
module_exit(test_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("zbzhuang");
MODULE_VERSION("V1.0");











