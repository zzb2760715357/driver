#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>


#include <linux/string.h>
#include <asm/uaccess.h>
#include <linux/errno.h>



#define DEBUG_SWITCH 1
#if DEBUG_SWITCH 
#define P_DEBUG(fmt,args...) printk("<1>""[%s]"fmt,__func__,##args)
#else
#define P_DEBUG(fmt,args...) printk("<7>""[%s]"fmt,__func__,##args)
#endif

#define DEV_SIZE  100

struct _test_t{
	char kbuf[DEV_SIZE];
	unsigned int major ;
	unsigned int minor ;
	unsigned int cur_size;
	dev_t devno;
	struct cdev  cdev_test;
};








const char * name = "zbzhuang";




int test_open (struct inode * node, struct file * filp)
{
	struct _test_t *dev;
	printk("kernel:-------%s--------\r\n",__func__);

	
	dev = container_of(node->i_cdev,struct _test_t,cdev_test);
	filp->private_data = dev;

	return 0;
}
int test_close (struct inode * node, struct file * filp)
{
	printk("kernel:-------%s--------\r\n",__func__);

	return 0;
}


ssize_t test_write (struct file * filp, char __user * buf, size_t count, loff_t * offset)
{
	int ret;
	//char kbuf[128];
	struct _test_t *dev = filp->private_data;

	printk("kernel:-------%s--------\r\n",__func__);
//	memcpy(kbuf,buf,count);
	if(copy_from_user(dev->kbuf,buf,count)){
		ret = -EFAULT;
	}else{
		ret = count;
		dev->cur_size += count;
		printk("kbuf is %s\r\n",dev->kbuf);
		printk("cur_size:[%d]\r\n",dev->cur_size);
	}
	//printk("kernel:kbuf is %s\r\n",kbuf);

	return ret;
}
ssize_t test_read (struct file *filp, const char __user * buf, size_t count, loff_t * offset)
{
	int ret;
	struct _test_t *dev = filp->private_data;
	printk("kernel:-------%s--------\r\n",__func__);
	

	if(!dev->cur_size){
		return 0;
	}

	


	//memcpy(buf,"kernel memcpy",count);
	if(copy_to_user(buf,dev->kbuf,count)){
		ret = -EFAULT;
	}else{
		ret = count;
		dev->cur_size -= count;
	}

	return ret;  //返回实际读取的字节或错误号
}




const struct file_operations test_fops = {
	.open = test_open,
	.release = test_close,
	.write = test_write,
	.read = test_read,
};

struct _test_t my_dev;

static int __init test_init(void)
{

	int ret = 0;
	my_dev.cur_size = 0;
	my_dev.major = 0;
	my_dev.minor = 0;

	printk("-------%s--------\r\n",__func__);
	printk("hello world\r\n");


	/*申请设备号*/
	if(my_dev.major){
		my_dev.devno = MKDEV(my_dev.major,my_dev.minor);	
		ret = register_chrdev_region(my_dev.devno,1,"test new driver");//查看在哪里打印的啊
	}else{
		ret =alloc_chrdev_region(&my_dev.devno,my_dev.minor,1,"alloc chrdev no");
		my_dev.major = MAJOR(my_dev.devno);
		my_dev.minor = MINOR(my_dev.devno);
		
	}

	P_DEBUG("major[%d]\tminor[%d]\r\n",my_dev.major,my_dev.minor);

	if(ret < 0 ){
		P_DEBUG("error register device\r\n");
		//ret = -EBUSY;
		goto err0;
	}else{
		P_DEBUG("Success register device\r\n");
	}
	

	/* 设备的注册 */
	cdev_init(&my_dev.cdev_test,&test_fops);
	my_dev.cdev_test.owner = THIS_MODULE;

	/*
		将cdev 和 devno关联起来
	*/
	ret = cdev_add(&my_dev.cdev_test,my_dev.devno,1);
	if(ret < 0){
		P_DEBUG("error register device\r\n");
		//ret = -ENODEV;
		goto err1;
	}else{
		P_DEBUG("Success cdev add\r\n");
	}

	return 0;
	

err1:
	unregister_chrdev_region(my_dev.devno,1);
err0:
	return ret;
}


static void __exit test_exit(void)
{

	cdev_del(&my_dev.cdev_test);
	unregister_chrdev_region(my_dev.major,1);

	printk("-------%s--------\r\n",__func__);
}



module_init(test_init);
module_exit(test_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("zbzhuang");
MODULE_VERSION("V1.0");











