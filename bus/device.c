#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/device.h>

extern struct bus_type usb_bus;


void	usb_dev_release(struct device *dev)
{
	printk("----------%s------------\r\n",__func__);
}


struct device usb_device = {
	.init_name = "usb_mouse",
	.bus = &usb_bus,
	.release = usb_dev_release,
};

#define VER_SIZE 100

char Version[VER_SIZE] = "zbzhuang device";

ssize_t show_device_version(struct device *dev, struct device_attribute *attr,
			char *buf)
{
	return snprintf(buf, VER_SIZE, "%s\n", Version);
}
ssize_t store_device_version(struct device *dev, struct device_attribute *attr,
			 const char *buf, size_t count)
{
	return snprintf(Version, VER_SIZE, "%s", buf);
}




static DEVICE_ATTR(version,S_IRUGO|S_IWUGO,
	show_device_version,store_device_version);
static int __init usb_device_init(void)
{
	int ret;

	printk("------------%s-------------\r\n",__func__);
	ret = device_register(&usb_device);
	if(ret){
		printk("device register failed!\r\n");
		goto err1;
	}


	ret = device_create_file(&usb_device,&dev_attr_version);
	if(ret){
		printk("device create file failed!\r\n");
		goto err2;
	}

	return 0;

err2:
	device_unregister(&usb_device);

err1:
	return ret;
	
}


static void __exit usb_device_exit(void)
{
	printk("------------%s-------------\r\n",__func__);
	device_remove_file(&usb_device,&dev_attr_version);
	device_unregister(&usb_device);


}



MODULE_LICENSE("GPL");
module_init(usb_device_init);
module_exit(usb_device_exit);




























