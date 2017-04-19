
#include <linux/module.h>
#include <linux/init.h>
#include <linux/gfp.h>
#include <linux/mm.h>

#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/fs.h>


#define SWITCH 1


struct page *p;
char *s;

char *temp;


static int __init mmu_init(void)
{
	unsigned long virt,phys;


	printk("---------%s------------\r\n",__func__);	

#if SWITCH
	p = alloc_pages(GFP_KERNEL,1);
	if(NULL == p){
		printk("Error alloc page\r\n");
		return -ENOMEM;
	}else{
		printk("Success alloc page\r\n");
	}

	printk("The first method\r\n");

	s = (char *)page_address(p);
#else
	s = (char *)__get_free_pages(GFP_KERNEL,1);
	if(NULL == s){
		printk("Error alloc page !\r\n");
		return -ENOMEM;
	}else{
		printk("Success alloc page and get virtual address\r\n");
	}
#endif

	phys = __pa((unsigned long)s);   //得到物理地址
	virt = (unsigned long)__va(phys);  //得到虚拟地址
	printk("p->virtual,s[%p]\r\n",s);
	printk("phys[%p]\r\n",(void *)phys);
	printk("virt[%p]\r\n",(void *)virt);

	memcpy(s,"hello mm",20);


	

	printk("hello kernel [%s]\r\n",s);


	printk("-------test kmalloc --------\r\n");
	temp = kmalloc(20,GFP_KERNEL);
	memcpy(temp,"hello kmalloc",20);
	printk("<kmalloc>%s\r\n",temp);

	return 0;
}

static void __exit mmu_exit(void)
{
	printk("---------%s------------\r\n",__func__);	
	kfree(temp);

#if SWITCH
	__free_pages(p,1);


#else
	free_pages((unsigned long)s,1);
#endif
}



MODULE_LICENSE("GPL");
module_init(mmu_init);
module_exit(mmu_exit);






