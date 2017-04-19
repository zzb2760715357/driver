#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>


#include <linux/string.h>
#include <asm/uaccess.h>
#include <linux/errno.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/poll.h>


#include "test_cmd.h"



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
	wait_queue_head_t test_queue;   //定义一个等待队列头
	wait_queue_head_t read_queue;   //定义一个等待队列头
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
	if(*offset >= DEV_SIZE){
		return count ?-ENXIO:0;
	}

	if(*offset + count > DEV_SIZE){
		count = DEV_SIZE - *offset;
	}

	if(copy_from_user(dev->kbuf,buf,count)){
		ret = -EFAULT;
	}else{
		ret = count;
		dev->cur_size += count;
		*offset += count;
		printk("kbuf is %s\r\n",dev->kbuf);
		printk("write %d bytes,cur_size:[%d]\r\n",count,dev->cur_size);
		wake_up_interruptible(&dev->read_queue);
		wake_up_interruptible(&dev->test_queue);
		P_DEBUG("wait up .....\r\n");
	}
	//printk("kernel:kbuf is %s\r\n",kbuf);

	return ret;
}
ssize_t test_read (struct file *filp, const char __user * buf, size_t count, loff_t * offset)
{
	int ret;
	struct _test_t *dev = filp->private_data;
	printk("kernel:-------%s--------\r\n",__func__);

	if(filp->f_flags &O_NONBLOCK){
		return -EAGAIN;
	}


	P_DEBUG("read data .....\n");
	if(wait_event_interruptible(dev->test_queue,dev->cur_size > 0)){
		return -ERESTARTSYS;
	}


	if(*offset >= DEV_SIZE){
		return count ? -ENXIO:0;
	}

	if(*offset + count > DEV_SIZE){
		count = DEV_SIZE - *offset;
	}
	

	

	


	//memcpy(buf,"kernel memcpy",count);
	if(copy_to_user(buf,dev->kbuf+*offset,count)){
		ret = -EFAULT;
	}else{
		ret = count;
		*offset += count;
		dev->cur_size -= count;
		P_DEBUG("read %d bytes,cur_size:[%d]\r\n",count,dev->cur_size);
	}

	return ret;  //返回实际读取的字节或错误号
}

loff_t test_llseek (struct file *filp, loff_t offset, int whence)
{

	loff_t new_pos;
	loff_t old_pos = filp->f_pos;

	switch(whence){
		case SEEK_SET:
			new_pos = offset;
			break;
		case SEEK_CUR:
			new_pos = old_pos + offset;
			break;
		case SEEK_END:
			new_pos = DEV_SIZE + offset;
			break;
		default:
			P_DEBUG("Unknow whence\n");
			return -EINVAL;
	}


	if(new_pos < 0 || new_pos > DEV_SIZE){
		P_DEBUG("f_pos failed\r\n");
		return -EINVAL;
	}

	filp->f_pos = new_pos;
	return new_pos;
	

}


long test_ioctl (struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	struct _test_t *dev = filp->private_data;
	struct ioctl_data val;


	if(_IOC_TYPE(cmd) != TEST_MAGIC)
		return -EINVAL;

	if(_IOC_NR(cmd)>TEST_MAX_NR)
		return -EINVAL;

	if(_IOC_DIR(cmd)&_IOC_READ)
		ret = access_ok(VERIFY_WRITE,(void __user *)arg,_IOC_SIZE(cmd));
	else if(_IOC_DIR(cmd)&_IOC_WRITE)
		ret = access_ok(VERIFY_READ,(void __user *)arg,_IOC_SIZE(cmd));
	if(!ret)
		return -EFAULT;

	switch(cmd){
		case TEST_CLEAR:
			memset(dev->kbuf,0,DEV_SIZE);
			dev->cur_size = 0;
			filp->f_pos = 0;
			ret = 0;
			P_DEBUG("clear the buff\r\n");
			break;
		case TEST_OFFSET:
			filp->f_pos += (int)arg;
			P_DEBUG("Change offset\r\n");
			ret = 0;
			break;

		case TEST_KBUF:
			
			#if 0
			P_DEBUG("set the kbuf\r\n");
			if(copy_from_user(&val,(struct ioctl_data*)arg,sizeof(struct ioctl_data))){
				ret = -EFAULT;
				goto err;
			}
			memset(dev->kbuf,0,DEV_SIZE);
			memcpy(dev->kbuf,val.buf,val.size);
			dev->cur_size = val.size;
			#else

			memcpy(dev->kbuf,((struct ioctl_data *)arg)->buf,((struct ioctl_data *)arg)->size);
			dev->cur_size = ((struct ioctl_data *)arg)->size;
			#endif
			
			
			filp->f_pos = 0;
			ret = 0;
			break;
		default:
			P_DEBUG("error cmd!\r\n");
			ret = -EINVAL;
			break;

	}


	return ret;

}


unsigned int test_poll (struct file * filp, struct poll_table_struct * table)
{
	struct _test_t *dev = filp->private_data;

	unsigned int mask = 0 ;

	poll_wait(filp,&dev->read_queue,table);

	if(dev->cur_size>0){
		mask |= POLLIN;
	}
	P_DEBUG("---mask[%d]----\r\n",mask);


	return mask;
}


const struct file_operations test_fops = {
	.open = test_open,
	.release = test_close,
	.write = test_write,
	.read = test_read,
	.llseek = test_llseek,
	.unlocked_ioctl = test_ioctl,
	.poll = test_poll,
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

	/*初始化等待队列头*/
	init_waitqueue_head(&my_dev.test_queue);
	init_waitqueue_head(&my_dev.read_queue);

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











