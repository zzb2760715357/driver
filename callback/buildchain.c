#include <linux/init.h>
#include <linux/module.h>
#include <linux/notifier.h>






/*
	֪ͨ�����ͷ���
*/

static RAW_NOTIFIER_HEAD(test_chain);

/*
	ע�ắ��
*/

int register_test_notifier(struct notifier_block * nb)
{
	return raw_notifier_chain_register(&test_chain,nb);
}

EXPORT_SYMBOL(register_test_notifier);

/*
	ж�غ���
*/
int unregister_test_notifier(struct notifier_block * nb)
{
	return raw_notifier_chain_unregister(&test_chain, nb);
}

EXPORT_SYMBOL(unregister_test_notifier);

/*
	֪ͨ������
*/
int test_notifier_call_chain(unsigned long val,void * v)
{
	return raw_notifier_call_chain(&test_chain,val,v);
}

EXPORT_SYMBOL(test_notifier_call_chain);


static int __init init_notifier(void)
{
	printk("---------%s---------\r\n",__func__);


	return 0;
}

static void __exit exit_notifier(void)
{
	printk("---------%s---------\r\n",__func__);


}







MODULE_LICENSE("GPL");
module_init(init_notifier);
module_exit(exit_notifier);















