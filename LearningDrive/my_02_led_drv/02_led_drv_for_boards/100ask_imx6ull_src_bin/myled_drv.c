
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/mutex.h>
#include <linux/wait.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <asm/io.h>

#include "led_opr.h"

static int major;
static struct class *led_class;
struct led_operations *p_led_opr;

#define MIN(a, b) (a < b ? a : b)

static ssize_t led_drv_read (struct file *file, char __user *buf, size_t size, loff_t *offset)
{
//	int status;
	
	//TODO

	struct inode *inode = file_inode(file);		
	
	int minor = iminor(inode);								//TODO
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	//read status from board
	p_led_opr->read_status(minor, buf);				//TODO

	return 0;
}

static ssize_t led_drv_write(struct file *file, const char __user *buf,
			 size_t count, loff_t *ppos)
{
	int err;
	char status;
	struct inode *inode = file_inode(file);
	int minor = iminor(inode);
	
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	err = copy_from_user(&status, buf, 1);

	/* 根据次设备号和status控制LED */
	p_led_opr->ctl(minor, status);
	
	return 1;

}

static int led_drv_open(struct inode *node, struct file *file)
{
	int minor = iminor(node);
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	/* 根据次设备号初始化LED */
	p_led_opr->init(minor);
	
	return 0;
}

static int led_drv_close (struct inode *node, struct file *file)
{
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}

static struct file_operations led_drv = {
	.owner		= THIS_MODULE,
	.read			= led_drv_read,
	.write		= led_drv_write,
	.open			= led_drv_open,
	.release	= led_drv_close,
};

/* 入口函数 */
static int __init led_init(void)
{
	int i, err;

	printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	
	major = register_chrdev(0, "my_led", &led_drv);

	led_class = class_create(THIS_MODULE, "myled");

	err = PTR_ERR(led_class);
	if (IS_ERR(led_class)) {
		printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
		unregister_chrdev(major, "100ask_led");
		return -1;
	}

	//必须先获得p_led_opr,才能使用num
	p_led_opr = get_board_led_opr();

	for(i = 0; i < p_led_opr->num; i++)
		device_create(led_class, NULL, MKDEV(major, i), NULL, "myled%d", i); /* /dev/myled */

	
	return 0;
}

static void __exit led_exit(void)
{
	int i;
	
	for(i = 0; i < p_led_opr->num; i++)
		device_destroy(led_class, MKDEV(major, i)); /* /dev/myled */
	
	class_destroy(led_class);
	
	unregister_chrdev(major, "my_led");
}

module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");


