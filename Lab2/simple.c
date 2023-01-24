#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

/* This function is called when the module is loaded. */
int simple_init(void)
{
    printk(KERN_INFO "Loading Module\n");
	return 0;
}
void printint(int*ptri)
{
	int num=*ptri;
	for(num;num>0;num--)
	{
			printk(KERN_INFO "%d\n",*ptri);
	}
}

/* This function is called when the module is removed. */
void simple_exit(void) {
	int i=3;
	printint(&i);
	printk(KERN_INFO "Removing Module\n");
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
