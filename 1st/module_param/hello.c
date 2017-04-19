#include <linux/init.h>
#include <linux/module.h>



#define DEBUG_SWITCH 0

#if DEBUG_SWITCH
	#define P_DEBUG(fmt,args...) printk("<1>""[%s]"fmt,__func__,##args)
#else
	#define P_DEBUG(fmt,args...) printk("<7>""[%s]"fmt,__func__,##args)
#endif

int num =  123;
char *name = "zbzhuang";





static int __init test_init(void)
{
	printk("-------%s--------\r\n",__func__);
	printk("hello world\r\n");
	printk("<0>""hello world! 0\r\n");
	printk("<1>""hello world! 1\r\n");
	printk("<2>""hello world! 2\r\n");
	printk("<3>""hello world! 3\r\n");
	printk("<4>""hello world! 4\r\n");
	printk("<5>""hello world! 5\r\n");
	printk("<6>""hello world! 6\r\n");
	printk("<7>""hello world! 7\r\n");

	printk("num = %d\r\n",num);
	printk("name = %s\r\n",name);

	return 0;
}


static void __exit test_exit(void)
{
	printk("-------%s--------\r\n",__func__);
}



module_init(test_init);
module_exit(test_exit);
module_param(num,int,0644);
module_param(name,charp,0644);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("zbzhuang");
MODULE_VERSION("V1.0");











