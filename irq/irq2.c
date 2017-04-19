#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>

int id = 321;


irqreturn_t irq_handler(int irqno, void *dev_id)
{
	int temp;

	temp = *((int *)dev_id);
	printk("temp = %d\r\n",temp);
	if(temp == id){
	printk("---------%s---------\r\n",__func__);
	printk("press the key2\r\n");
	}
	
	return IRQ_HANDLED;
}


static int __init test_init(void)
{
	int ret;

	printk("---------%s---------\r\n",__func__);


	ret = request_irq(IRQ_EINT(1),irq_handler,IRQF_TRIGGER_FALLING|IRQF_TRIGGER_RISING|IRQF_SHARED,"KEY EINT2",&id);
	if(ret){
		printk("Error request_irq\r\n");

		return -1;
	}

	
	


	return 0;
}


static void __exit test_exit(void)
{
	printk("-----------%s------------\r\n",__func__);

	free_irq(IRQ_EINT(1),&id);
}


MODULE_LICENSE("GPL");
module_init(test_init);
module_exit(test_exit);




