
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

#define LED_NUM 2

static int major;
static struct class *led_class;
struct led_operations *p_led_opr;

#define MIN(a, b) (a < b ? a : b)

////
///* registers */
//// IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 地址：0x02290000 + 0x14
//static volatile unsigned int *IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3;
//
//// GPIO5_GDIR 地址：0x020AC004
//static volatile unsigned int *GPIO5_GDIR;
//
////GPIO5_DR 地址：0x020AC000
//static volatile unsigned int *GPIO5_DR;

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
//	int ret;
//	char val;
//	/*copy_from_user : get data from user */
//	ret = copy_from_user(&val, buf, 1);
//	// : set led status
//	if(val)
//	{
//		//set led on : set 0
//		*GPIO5_DR &= ~(1<<3);
//	}
//	else
//	{
//		//set led off : set 1
//		*GPIO5_DR |= (1<<3);
//	}
//	return 0;	

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
//	//config gpio as output
//	*IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 &= ~0xf;
//	*IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 |= 0x5;
//	*GPIO5_GDIR |= (1<<3);
//	return 0;
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


	for(i = 0; i < LED_NUM; i++)
		device_create(led_class, NULL, MKDEV(major, i), NULL, "myled%d", i); /* /dev/myled */

//	/* ioremap */
//	// IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 地址：0x02290000 + 0x14，实际映射为1页，即4k
//	IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 = ioremap(0x02290000 + 0x14, 4);
//	
//	// GPIO5_GDIR 地址：0x020AC004
//	GPIO5_GDIR = ioremap(0x020AC004, 4);
//	
//	//GPIO5_DR 地址：0x020AC000
//	GPIO5_DR  = ioremap(0x020AC000, 4);
//	//主设备号为major，次设备号为0
//	//设备节点为dev/myled
//	device_create(led_class, NULL, MKDEV(major, 0), NULL, "myled"); /* /dev/myled */
	
	p_led_opr = get_board_led_opr();
	return 0;
}

static void __exit led_exit(void)
{
	int i;
//	iounmap(IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3);
//	iounmap(GPIO5_GDIR);
//	iounmap(GPIO5_DR);
	
	for(i = 0; i < LED_NUM; i++)
		device_destroy(led_class, MKDEV(major, i)); /* /dev/myled */
//	device_destroy(led_class, MKDEV(major, 0));
	
	class_destroy(led_class);
	
	unregister_chrdev(major, "my_led");
}

module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");


