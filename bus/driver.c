#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>

extern struct bus_type usb_bus;

void usb_dev_release(struct device *dev)	//ж�غ���û�иɾ��������
{
		printk("<kernel> release\n");
}

struct device usb_device = {
	.init_name = "usb_mouse",
	.bus = &usb_bus,			//ָ�����豸������,��/sys/bus/usb
	.release = usb_dev_release,	//����Ҫ����release��������Ȼж��ʱ�����
};

static int __init usb_device_init(void)
{
	int ret;

	ret = device_register(&usb_device);
	if(ret){
		printk("device register failed!\n");
		return ret;	
	}

	printk("usb device init\n");
	return 0;
}

static void __exit usb_device_exit(void)
{
	device_unregister(&usb_device);
	printk("usb device bye!\n");
}

module_init(usb_device_init);
module_exit(usb_device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiao bai");
