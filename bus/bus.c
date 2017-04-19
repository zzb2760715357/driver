#include <linux/module.h>
#include <linux/init.h>

#include <linux/device.h>

int usb_bus_match(struct device *dev, struct device_driver *drv)
{
	if(!strcmp(dev->init_name, drv->name)){
		printk("match success\n");
		return 1;
	}else{
		printk("match failed\n");
		return 0;
	}
}

struct bus_type usb_bus = {
	.name = "usb",	//ע��ɹ�����/sys/busĿ¼�¿���Ŀ¼usb
	.match = usb_bus_match,
};

static int __init usb_bus_init(void)
{
	int ret;
	/*����ע�ᣬ�����ⷵ��ֵ*/
	ret = bus_register(&usb_bus);
	if(ret){
		printk("bus register failed!\n");
		return ret;	
	}

	printk("usb bus init\n");
	return 0;
}

static void __exit usb_bus_exit(void)
{
	bus_unregister(&usb_bus);
	printk("usb bus bye!\n");
}

EXPORT_SYMBOL(usb_bus);		//��������usb_bus����device��driver��������

module_init(usb_bus_init);
module_exit(usb_bus_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiao bai");
