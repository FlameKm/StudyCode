﻿#include <linux/module.h>
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

struct gpio_desc{
	int gpio;
	int irq;
    char name[128];
    int key;
	struct timer_list key_timer;
} ;

static struct gpio_desc *gpios;
static int count;

/* 主设备号                                                                 */
static int major = 0;
static struct class *gpio_class;

/* 环形缓冲区 */
#define BUF_LEN 128
static unsigned char g_keys[BUF_LEN];
static int r, w;

struct fasync_struct *button_fasync;

static u64 g_irda_irq_times[68];
static int g_irda_irq_cnt = 0;

#define NEXT_POS(x) ((x+1) % BUF_LEN)

static int is_key_buf_empty(void)
{
	return (r == w);
}

static int is_key_buf_full(void)
{
	return (r == NEXT_POS(w));
}

static void put_key(unsigned char key)
{
	if (!is_key_buf_full())
	{
		g_keys[w] = key;
		w = NEXT_POS(w);
	}
}

static unsigned char get_key(void)
{
	unsigned char key = 0;
	if (!is_key_buf_empty())
	{
		key = g_keys[r];
		r = NEXT_POS(r);
	}
	return key;
}


static DECLARE_WAIT_QUEUE_HEAD(gpio_wait);

// static void key_timer_expire(struct timer_list *t)
static void key_timer_expire(unsigned long data)
{
	/* 超时 */
	g_irda_irq_cnt = 0;
	put_key(-1);
	put_key(-1);
	wake_up_interruptible(&gpio_wait);
	kill_fasync(&button_fasync, SIGIO, POLL_IN);
}


/* 实现对应的open/read/write等函数，填入file_operations结构体                   */
static ssize_t irda_read (struct file *file, char __user *buf, size_t size, loff_t *offset)
{
	unsigned char kern_buf[2] ;
	int err;

	if (size != 2)
		return -EINVAL;

	if (is_key_buf_empty() && (file->f_flags & O_NONBLOCK))
		return -EAGAIN;
	
	wait_event_interruptible(gpio_wait, !is_key_buf_empty());
	kern_buf[0] = get_key();  /* device */
	kern_buf[1] = get_key();  /* data   */
	
	if (kern_buf[0] == (unsigned char)-1  && kern_buf[1] == (unsigned char)-1)
		return -EIO;

	err = copy_to_user(buf, kern_buf, 2);

    return 2;    
}


static unsigned int irda_poll(struct file *fp, poll_table * wait)
{
	//printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	poll_wait(fp, &gpio_wait, wait);
	return is_key_buf_empty() ? 0 : POLLIN | POLLRDNORM;
}

static int irda_fasync(int fd, struct file *file, int on)
{
	if (fasync_helper(fd, file, on, &button_fasync) >= 0)
		return 0;
	else
		return -EIO;
}


/* 定义自己的file_operations结构体                                              */
static struct file_operations gpio_key_drv = {
	.owner	 = THIS_MODULE,
	.read    = irda_read,
	.poll    = irda_poll,
	.fasync  = irda_fasync,
};

static void parse_irda_datas(void)
{
	u64 time;
	int i;
	int m, n;
	unsigned char datas[4];
	unsigned char data = 0;
	int bits = 0;
	int byte = 0;

	/* 1. 判断前导码 : 9ms的低脉冲, 4.5ms高脉冲  */
	time = g_irda_irq_times[1] - g_irda_irq_times[0];
	if (time < 8000000 || time > 10000000)
	{
		goto err;
	}

	time = g_irda_irq_times[2] - g_irda_irq_times[1];
	if (time < 3500000 || time > 55000000)
	{
		goto err;
	}

	/* 2. 解析数据 */
	for (i = 0; i < 32; i++)
	{
		m = 3 + i*2;
		n = m+1;
		time = g_irda_irq_times[n] - g_irda_irq_times[m];
		data <<= 1;
		bits++;
		if (time > 1000000)
		{
			/* 得到了数据1 */
			data |= 1;
		}

		if (bits == 8)
		{
			datas[byte] = data;
			byte++;
			data = 0;
			bits = 0;
		}
	}

	/* 判断数据正误 */
	datas[1] = ~datas[1];
	datas[3] = ~datas[3];
	
	if ((datas[0] != datas[1]) || (datas[2] != datas[3]))
	{
		printk("data verify err: %02x %02x %02x %02x\n", datas[0], datas[1], datas[2], datas[3]);
		goto err;
	}

	put_key(datas[0]);
	put_key(datas[2]);
	wake_up_interruptible(&gpio_wait);
	kill_fasync(&button_fasync, SIGIO, POLL_IN);
	return;

err:
	g_irda_irq_cnt = 0;
	put_key(-1);
	put_key(-1);
	wake_up_interruptible(&gpio_wait);
	kill_fasync(&button_fasync, SIGIO, POLL_IN);
}

static int get_irda_repeat_datas(void)
{
	u64 time;

	/* 1. 判断重复码 : 9ms的低脉冲, 2.25ms高脉冲  */
	time = g_irda_irq_times[1] - g_irda_irq_times[0];
	if (time < 8000000 || time > 10000000)
	{
		return -1;
	}

	time = g_irda_irq_times[2] - g_irda_irq_times[1];
	if (time < 2000000 || time > 2500000)
	{
		return -1;
	}	

	return 0;
}

static irqreturn_t gpio_key_isr(int irq, void *dev_id)
{
	struct gpio_desc *gpio_desc = dev_id;
	u64 time;
	
	/* 1. 记录中断发生的时刻 */	
	time = ktime_get_ns();
	g_irda_irq_times[g_irda_irq_cnt] = time;

	/* 2. 累计中断次数 */
	g_irda_irq_cnt++;

	/* 3. 次数达标后, 删除定时器, 解析数据, 放入buffer, 唤醒APP */
	if (g_irda_irq_cnt == 4)
	{
		/* 是否重复码 */
		if (0 == get_irda_repeat_datas())
		{
			/* device: 0, val: 0, 表示重复码 */
			put_key(0);
			put_key(0);
			wake_up_interruptible(&gpio_wait);
			kill_fasync(&button_fasync, SIGIO, POLL_IN);
			del_timer(&gpio_desc->key_timer);
			g_irda_irq_cnt = 0;
			return IRQ_HANDLED;
		}
	}
	if (g_irda_irq_cnt == 68)
	{
		parse_irda_datas();
		del_timer(&gpio_desc->key_timer);
		g_irda_irq_cnt = 0;
		return IRQ_HANDLED;
	}

	/* 4. 启动定时器 */
	mod_timer(&gpio_desc->key_timer, jiffies + msecs_to_jiffies(100));
	return IRQ_HANDLED;
}


/* 在入口函数 */
static int gpio_drv_probe(struct platform_device *pdev)
{
    int err = 0;
    int i;
	struct device_node *np = pdev->dev.of_node;
	struct resource *res;
    
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	

	/* 从platfrom_device获得引脚信息 
	 * 1. pdev来自c文件
     * 2. pdev来自设备树
	 */
	
	if (np)
	{
		/* pdev来自设备树 : 示例
        reg_usb_ltemodule: regulator@1 {
            compatible = "100ask,gpiodemo";
            gpios = <&gpio5 5 GPIO_ACTIVE_HIGH>, <&gpio5 3 GPIO_ACTIVE_HIGH>;
        };
		*/
		count = of_gpio_count(np);
		if (!count)
			return -EINVAL;

		gpios = kmalloc(count * sizeof(struct gpio_desc), GFP_KERNEL);
		for (i = 0; i < count; i++)
		{
			gpios[i].gpio = of_get_gpio(np, i);
			sprintf(gpios[i].name, "%s_pin_%d", np->name, i);
		}
	}
	else
	{
		/* pdev来自c文件 
		static struct resource omap16xx_gpio3_resources[] = {
			{
					.start  = 115,
					.end    = 115,
					.flags  = IORESOURCE_IRQ,
			},
			{
					.start  = 118,
					.end    = 118,
					.flags  = IORESOURCE_IRQ,
			},		};		
		*/
		count = 0;
		while (1)
		{
			res = platform_get_resource(pdev, IORESOURCE_IRQ, count);
			if (res)
			{
				count++;
			}
			else
			{
				break;
			}
		}

		if (!count)
			return -EINVAL;

		gpios = kmalloc(count * sizeof(struct gpio_desc), GFP_KERNEL);
		for (i = 0; i < count; i++)
		{
			res = platform_get_resource(pdev, IORESOURCE_IRQ, i);
			gpios[i].gpio = res->start;
			sprintf(gpios[i].name, "%s_pin_%d", pdev->name, i);
		}

	}

	for (i = 0; i < count; i++)
	{		
		gpios[i].irq  = gpio_to_irq(gpios[i].gpio);

		setup_timer(&gpios[i].key_timer, key_timer_expire, (unsigned long)&gpios[i]);
	 	//timer_setup(&gpios[i].key_timer, key_timer_expire, 0);
		err = request_irq(gpios[i].irq, gpio_key_isr, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, gpios[i].name, &gpios[i]);
	}

	/* 注册file_operations 	*/
	major = register_chrdev(0, "100ask_irda", &gpio_key_drv);  /* /dev/gpio_desc */

	gpio_class = class_create(THIS_MODULE, "100ask_irda_class");
	if (IS_ERR(gpio_class)) {
		printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
		unregister_chrdev(major, "100ask_gpio_key");
		return PTR_ERR(gpio_class);
	}

	device_create(gpio_class, NULL, MKDEV(major, 0), NULL, "irda"); /* /dev/irda */
	
	return err;
}

/* 有入口函数就应该有出口函数：卸载驱动程序时，就会去调用这个出口函数
 */
static int gpio_drv_remove(struct platform_device *pdev)
{
    int i;
    
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

	device_destroy(gpio_class, MKDEV(major, 0));
	class_destroy(gpio_class);
	unregister_chrdev(major, "100ask_irda");

	for (i = 0; i < count; i++)
	{
		free_irq(gpios[i].irq, &gpios[i]);
		del_timer(&gpios[i].key_timer);
	}
	return 0;
}

static const struct of_device_id gpio_dt_ids[] = {
        { .compatible = "100ask,gpiodemo", },
        { /* sentinel */ }
};

static struct platform_driver gpio_platform_driver = {
	.driver		= {
		.name	= "100ask_gpio_plat_drv",
		.of_match_table = gpio_dt_ids,
	},
	.probe		= gpio_drv_probe,
	.remove		= gpio_drv_remove,
};

static int __init gpio_drv_init(void)
{
	/* 注册platform_driver */
	return platform_driver_register(&gpio_platform_driver);
}

static void __exit gpio_drv_exit(void)
{
	/* 反注册platform_driver */
	platform_driver_unregister(&gpio_platform_driver);
}

/* 7. 其他完善：提供设备信息，自动创建设备节点                                     */

module_init(gpio_drv_init);
module_exit(gpio_drv_exit);

MODULE_LICENSE("GPL");


