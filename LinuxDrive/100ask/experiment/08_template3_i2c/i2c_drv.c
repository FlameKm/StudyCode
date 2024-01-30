#include "linux/i2c.h"
#include <linux/module.h>
#include <linux/poll.h>

#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/gfp.h>
#include <linux/gpio/consumer.h>
#include <linux/platform_device.h>
#include <linux/of_gpio.h>
#include <linux/of_irq.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/slab.h>
#include <linux/fcntl.h>
#include <linux/timer.h>

/* 主设备号                                                                 */
static int major = 0;
static struct class *my_i2c_class;

struct i2c_client *g_client;

static DECLARE_WAIT_QUEUE_HEAD(gpio_wait);
struct fasync_struct *i2c_fasync;


/* 实现对应的open/read/write等函数，填入file_operations结构体                   */
static ssize_t i2c_drv_read (struct file *file, char __user *buf, size_t size, loff_t *offset)
{
	int err;

	struct i2c_msg msgs[2];

	/* 初始化i2c_msg */

	err = i2c_transfer(g_client->adapter, msgs, 2);

	/* copy_to_user  */
	
	return 0;
}

static ssize_t i2c_drv_write(struct file *file, const char __user *buf, size_t size, loff_t *offset)
{
	int err;

	/* copy_from_user  */


	struct i2c_msg msgs[2];

	/* 初始化i2c_msg */

	err = i2c_transfer(g_client->adapter, msgs, 2);

	
	return 0;    
}


static unsigned int i2c_drv_poll(struct file *fp, poll_table * wait)
{
	//printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	poll_wait(fp, &gpio_wait, wait);
	//return is_key_buf_empty() ? 0 : POLLIN | POLLRDNORM;
	return 0;
}

static int i2c_drv_fasync(int fd, struct file *file, int on)
{
	if (fasync_helper(fd, file, on, &i2c_fasync) >= 0)
		return 0;
	else
		return -EIO;
}


/* 定义自己的file_operations结构体                                              */
static struct file_operations i2c_drv_fops = {
	.owner	 = THIS_MODULE,
	.read    = i2c_drv_read,
	.write   = i2c_drv_write,
	.poll    = i2c_drv_poll,
	.fasync  = i2c_drv_fasync,
};


static int i2c_drv_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	// struct device_node *np = client->dev.of_node;
	// struct i2c_adapter *adapter = client->adapter;

	/* 记录client */
	g_client = client;

	/* 注册字符设备 */
	/* 注册file_operations 	*/
	major = register_chrdev(0, "100ask_i2c", &i2c_drv_fops);  /* /dev/gpio_desc */

	my_i2c_class = class_create(THIS_MODULE, "100ask_i2c_class");
	if (IS_ERR(my_i2c_class)) {
		printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
		unregister_chrdev(major, "100ask_i2c");
		return PTR_ERR(my_i2c_class);
	}

	device_create(my_i2c_class, NULL, MKDEV(major, 0), NULL, "myi2c"); /* /dev/myi2c */
	
	return 0;
}

static int i2c_drv_remove(struct i2c_client *client)
{
	/* 反注册字符设备 */
	device_destroy(my_i2c_class, MKDEV(major, 0));
	class_destroy(my_i2c_class);
	unregister_chrdev(major, "100ask_i2c");

	return 0;
}

static const struct of_device_id myi2c_dt_match[] = {
	{ .compatible = "100ask,i2cdev" },
	{},
};
static struct i2c_driver my_i2c_driver = {
	.driver = {
		   .name = "100ask_i2c_drv",
		   .owner = THIS_MODULE,
		   .of_match_table = myi2c_dt_match,
	},
	.probe = i2c_drv_probe,
	.remove = i2c_drv_remove,
};


static int __init i2c_drv_init(void)
{
	/* 注册i2c_driver */
	return i2c_add_driver(&my_i2c_driver);
}

static void __exit i2c_drv_exit(void)
{
	/* 反注册i2c_driver */
	i2c_del_driver(&my_i2c_driver);
}

/* 7. 其他完善：提供设备信息，自动创建设备节点                                     */

module_init(i2c_drv_init);
module_exit(i2c_drv_exit);

MODULE_LICENSE("GPL");


