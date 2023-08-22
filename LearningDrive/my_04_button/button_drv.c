#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/bitrev.h>
#include <linux/mutex.h>
#include <linux/uaccess.h>
#include <linux/io.h>

#include <pcmcia/cistpl.h>
#include <pcmcia/cisreg.h>
#include <pcmcia/ciscode.h>
#include <pcmcia/ds.h>


#include "button_drv.h"
static struct button_operations *p_button_opr;
static int major;
static struct class *button_class;

void register_button_operations(struct button_operations *opr)
{
  int i;

  p_button_opr = opr;
	for(i = 0; i < p_button_opr->count; i++)
	{
		device_create(button_class, NULL, MKDEV(major, i), NULL, "button%d", i);
	}
}

void unregister_button_operations(void)
{
	int i;
	for(i = 0; i < p_button_opr->count; i++)
	{
		device_destroy(button_class,  MKDEV(major, i));
	}
}
EXPORT_SYMBOL(register_button_operations);
EXPORT_SYMBOL(unregister_button_operations);



static int button_drv_open(struct inode *inode, struct file *filp)
{
	int minor = iminor(inode);;
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	p_button_opr->init(minor);
	return 0;
}
static ssize_t button_drv_write(struct file *filp, const char __user *buf,
			 size_t count, loff_t *ppos)
{
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 1;
}
static ssize_t bution_drv_read(struct file *filp, __user char *buf, size_t count,
			loff_t *ppos)
{
	unsigned int minor = iminor(file_inode(filp));
	char level;
	int err;
	level = p_button_opr->read(minor);
	err = copy_to_user(buf, &level, 1);
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 1;
}
static struct file_operations button_drv_operation = {
	.owner	= THIS_MODULE,
	.open = button_drv_open,
	.write = button_drv_write,
	.read = bution_drv_read,
	
};

static int button_drv_init(void)
{

	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	major = register_chrdev(0, "button_drv", &button_drv_operation);

	button_class = class_create(THIS_MODULE, "button_drv");
	
	return 0;
}

static void button_drv_exit(void)
{
	class_destroy(button_class);

	unregister_chrdev(major, "button_drv");
}


module_init(button_drv_init);
module_exit(button_drv_exit);

MODULE_LICENSE("GPL");
