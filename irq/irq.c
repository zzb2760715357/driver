#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>

#include <linux/workqueue.h>


#define SWITCH 0




#if SWITCH
struct tasklet_struct test_tasklet;

void tasklet_fun(unsigned long data)
{
	printk("---------%s---------\r\n",__func__);
	printk("data[%ld]\r\n",data);
}

#else
struct workqueue_struct *test_wq;
struct work_struct test_work;
void test_func(struct work_struct *work)
{
	printk("event hello test work!\n");
}


#endif


int id =123;
irqreturn_t irq_handler(int irqno, void *dev_id)
{
	int temp;

	temp = *((int *)dev_id);

	if(temp == 123){


	printk("---------%s---------\r\n",__func__);
	printk("press the key1\r\n");
	#if SWITCH
	tasklet_schedule(&test_tasklet);
	#else
	//queue_work(test_wq,&test_work);
	schedule_work(&test_work);
	#endif
	
		}
	return IRQ_HANDLED;
}


static int __init test_init(void)
{
	int ret;

	printk("---------%s---------\r\n",__func__);
	#if SWITCH
	tasklet_init(&test_tasklet,tasklet_fun,(unsigned long)123);
	#else
	test_wq = create_workqueue("test workqueue");
	INIT_WORK(&test_work,test_func);
	#endif
	ret = request_irq(IRQ_EINT(1),irq_handler,IRQF_TRIGGER_FALLING|IRQF_TRIGGER_RISING|IRQF_SHARED,"KEY EINT1",&id);
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
	#if SWITCH
	tasklet_kill(&test_tasklet);
	#else
	flush_workqueue(test_wq);
	destroy_workqueue(test_wq);
	#endif
}


MODULE_LICENSE("GPL");
module_init(test_init);
module_exit(test_exit);




