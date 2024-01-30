#include "asm/cacheflush.h"
#include <linux/spi/spi.h>
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
static struct class *my_spi_class;

static struct spi_device *g_spi;

static DECLARE_WAIT_QUEUE_HEAD(gpio_wait);
struct fasync_struct *spi_fasync;


/* 实现对应的open/read/write等函数，填入file_operations结构体                   */
static ssize_t spi_drv_read (struct file *file, char __user *buf, size_t size, loff_t *offset)
{
	// int err;

	// struct spi_transfer msgs[2];

	/* 初始化 spi_transfer */

	// static inline int
    //   spi_sync_transfer(struct spi_device *spi, struct spi_transfer *xfers,
	//   unsigned int num_xfers);

	/* copy_to_user  */
	
	return 0;
}

static ssize_t spi_drv_write(struct file *file, const char __user *buf, size_t size, loff_t *offset)
{
	int err;
	short val;
	unsigned char ker_buf[2];

	struct spi_transfer t;

	memset(&t, 0, sizeof(t));

	if (size != 2)
		return -EINVAL;	

	/* copy_from_user  */
	err = copy_from_user(&val, buf, size);
	val <<= 2;
	val &= 0x0fff;

	ker_buf[0] = val >> 8;
	ker_buf[1] = val;


	/* 初始化 spi_transfer */
	t.tx_buf = ker_buf;
	t.len    = 2;

	err = spi_sync_transfer(g_spi, &t, 1);
	
	return size;    
}


static unsigned int spi_drv_poll(struct file *fp, poll_table * wait)
{
	//printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	poll_wait(fp, &gpio_wait, wait);
	//return is_key_buf_empty() ? 0 : POLLIN | POLLRDNORM;
	return 0;
}

static int spi_drv_fasync(int fd, struct file *file, int on)
{
	if (fasync_helper(fd, file, on, &spi_fasync) >= 0)
		return 0;
	else
		return -EIO;
}


/* 定义自己的file_operations结构体                                              */
static struct file_operations spi_drv_fops = {
	.owner	 = THIS_MODULE,
	.read    = spi_drv_read,
	.write   = spi_drv_write,
	.poll    = spi_drv_poll,
	.fasync  = spi_drv_fasync,
};


static int spi_drv_probe(struct spi_device *spi)
{
	// struct device_node *np = client->dev.of_node;

	/* 记录spi_device */
	g_spi = spi;

	/* 注册字符设备 */
	/* 注册file_operations 	*/
	major = register_chrdev(0, "100ask_spi", &spi_drv_fops);  /* /dev/gpio_desc */

	my_spi_class = class_create(THIS_MODULE, "100ask_spi_class");
	if (IS_ERR(my_spi_class)) {
		printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
		unregister_chrdev(major, "100ask_spi");
		return PTR_ERR(my_spi_class);
	}

	device_create(my_spi_class, NULL, MKDEV(major, 0), NULL, "myspi"); /* /dev/myspi */
	
	return 0;
}

static int spi_drv_remove(struct spi_device *spi)
{
	/* 反注册字符设备 */
	device_destroy(my_spi_class, MKDEV(major, 0));
	class_destroy(my_spi_class);
	unregister_chrdev(major, "100ask_spi");

	return 0;
}

static const struct of_device_id myspi_dt_match[] = {
	{ .compatible = "100ask,spidev" },
	{},
};
static struct spi_driver my_spi_driver = {
	.driver = {
		   .name = "100ask_spi_drv",
		   .owner = THIS_MODULE,
		   .of_match_table = myspi_dt_match,
	},
	.probe = spi_drv_probe,
	.remove = spi_drv_remove,
};


static int __init spi_drv_init(void)
{
	/* 注册spi_driver */
	return spi_register_driver(&my_spi_driver);
}

static void __exit spi_drv_exit(void)
{
	/* 反注册spi_driver */
	spi_unregister_driver(&my_spi_driver);
}

/* 7. 其他完善：提供设备信息，自动创建设备节点                                     */

module_init(spi_drv_init);
module_exit(spi_drv_exit);

MODULE_LICENSE("GPL");


