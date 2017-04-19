#include <linux/module.h>
#include <linux/init.h>

#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/errno.h>

#include <asm/io.h>
#include <linux/ioport.h>

#include "led_ioctl.h"

#define DEBUG_SWITCH    1
#if DEBUG_SWITCH
	#define P_DEBUG(fmt, args...)   printk("<1>" "<kernel>[%s]"fmt, __FUNCTION__, ##args)
#else
	#define P_DEBUG(fmt, args...)   printk("<7>" "<kernel>[%s]"fmt, __FUNCTION__, ##args)
#endif

#define GPC0CON	(*(volatile unsigned long *)0xe0200060)
#define GPC0DAT	(*(volatile unsigned long *)0xe0200064)


struct _led_t{
	//hardware obb
	unsigned long virt, phys;
	unsigned long gpecon, gpedat, gpeup;
	unsigned long reg;
	struct resource *led_resource;

	void (*config)(struct _led_t *);
	void (*on)(struct _led_t *);
	void (*off)(struct _led_t *);
	
	//kernel oob
	dev_t devno;
	struct cdev led_cdev;
	struct semaphore led_sem;
};

void s3c_led_config(struct _led_t *led)
{
	printk("kernel:-------%s--------\r\n",__func__);
#if 0
	led->reg = ioread32(led->gpecon);
	led->reg &= ~(3 << 24);
	led->reg |= (1 << 24);
	iowrite32(led->reg, led->gpecon);
	
	led->reg = ioread32(led->gpeup);
	led->reg &= ~(3 << 12);
	iowrite32(led->reg, led->gpeup);
#else
	GPC0CON |= (0x1<<12)|(0x1<<16);	//设置GPC0_3,4为输出口
	GPC0DAT =0X00;					//初始化GPC0_3,4引脚数据
#endif

}

void s3c_led_on(struct _led_t *led)
{
	printk("kernel:-------%s--------\r\n",__func__);
	#if 0
	led->reg = ioread32(led->gpedat);
	led->reg &= ~(1 << 12);
	iowrite32(led->reg, led->gpedat);
	#else
	GPC0DAT |=(1<<3);

	#endif
}

void s3c_led_off(struct _led_t *led)
{
	printk("kernel:-------%s--------\r\n",__func__);
	#if 0
	led->reg = ioread32(led->gpedat);
	led->reg |= (1 << 12);
	iowrite32(led->reg, led->gpedat);
	#else
	GPC0DAT &= ~(1<<3);
	#endif
}

int init_led_device(struct _led_t *led)
{
	led->phys = 0xE0200060;

	led->led_resource = request_mem_region(led->phys, 0x0c, "LED_MEM");
	if(NULL == led->led_resource){
		return - 1;
	}

	led->virt = (unsigned long)ioremap(led->phys, 0x0c);

	led->gpecon = led->virt + 0x40;
	led->gpedat = led->virt + 0x44;
	led->gpeup = led->virt + 0x48;

	led->config = s3c_led_config;
	led->on = s3c_led_on;
	led->off = s3c_led_off;

	sema_init(&led->led_sem,2);

	return 0;
}

void exit_led_device(struct _led_t *led)
{
	printk("kernel:-------%s--------\r\n",__func__);

	if(NULL != led->led_resource){
		iounmap((void *)led->virt);
		release_mem_region(led->phys, 0x0c);
	}
}

int s3c_led_open (struct inode *node, struct file *filp)
{

	struct _led_t *dev;
	printk("kernel:-------%s--------\r\n",__func__);

	dev = container_of(node->i_cdev,struct _led_t,led_cdev);
	filp->private_data = dev;

	if(down_trylock(&dev->led_sem)){
		P_DEBUG("led_busy!\r\n");
		return -EBUSY;
	}


	return 0;
}

int s3c_led_close (struct inode * node, struct file * filp)
{
	struct _led_t *dev = filp->private_data;
	printk("kernel:-------%s--------\r\n",__func__);
	up(&dev->led_sem);
	return 0;
}

long s3c_led_ioctl (struct file *filp, unsigned int cmd, unsigned long args)
//int s3c_led_ioctl(struct inode *node, struct file *filp, unsigned int cmd, unsigned long args)
{
	int ret;
	struct _led_t *dev = filp->private_data;
	switch(cmd){
		case LED_CONF:
			dev->config(dev);
			break;
		case LED_ON:
			dev->on(dev);
			break;
		case LED_OFF:
			dev->off(dev);
			break;
		default:
			P_DEBUG("unknow cmd!\n");
			ret = - EINVAL;
			goto err0;
	}
	return 0;

err0:
	return ret;
}

struct _led_t my_led;
struct file_operations s3c_led_fops = {
	.unlocked_ioctl = s3c_led_ioctl,
	.open = s3c_led_open,
	.release = s3c_led_close,
};

static int __init led_driver__init(void)	//模块初始化函数
{
	int ret;

	ret = init_led_device(&my_led);
	if (ret){
		P_DEBUG("request mem error!\n");
		ret = - ENOMEM;
		goto err0;
	}

	ret = alloc_chrdev_region(&my_led.devno, 0, 1, "s3c_led_driver");
	if (ret){
		P_DEBUG("alloc chrdev failed!\n");
		goto err1;
	}
	P_DEBUG("major[%d], minor[%d]\n", MAJOR(my_led.devno), MINOR(my_led.devno));

	cdev_init(&my_led.led_cdev, &s3c_led_fops);

	ret = cdev_add(&my_led.led_cdev, my_led.devno, 1);
	if (ret){
		P_DEBUG("cdev_add failed!\n");
		goto err2;
	}

	P_DEBUG("hello led!\n");
	return 0;

err2:
	unregister_chrdev_region(my_led.devno, 1);
err1:
	exit_led_device(&my_led);
err0:
	return ret;
}

static void __exit led_driver__exit(void)		//模块卸载函数
{
	unregister_chrdev_region(my_led.devno, 1);
	exit_led_device(&my_led);
	P_DEBUG("bye\n");
}

module_init(led_driver__init);
module_exit(led_driver__exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xoao bai");
MODULE_VERSION("v0.1");
