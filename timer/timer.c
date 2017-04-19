#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/sched.h>



struct timer_list my_timer;


void timer_func(unsigned long data)
{
	printk("-----------%s------------\r\n",__func__);
	printk("time out![%ld][%s]\r\n",data,current->comm);

	//my_timer.expires = jiffies + 2*HZ;
	//add_timer(&my_timer);
	mod_timer(&my_timer,jiffies + 2*HZ);
}


static int __init timer_init(void)
{
	printk("-----------%s------------\r\n",__func__);
	init_timer(&my_timer);
	my_timer.expires = jiffies + 5*HZ;
	my_timer.function = timer_func,
	my_timer.data = (unsigned long)99;
	add_timer(&my_timer);
	printk("hello timer,current->comm[%s]\r\n",current->comm);



	return 0;
}

static void __exit timer_exit(void)
{
	printk("-----------%s------------\r\n",__func__);
	del_timer(&my_timer);


}





MODULE_LICENSE("GPL");
module_init(timer_init);
module_exit(timer_exit);



















