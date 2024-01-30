#include <linux/module.h>
#include <linux/poll.h>
#include <linux/delay.h>
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

/* 马达引脚设置数字 */
static int g_motor_pin_ctrl[8]= {0x2,0x3,0x1,0x9,0x8,0xc,0x4,0x6};
static int g_motor_index = 0;

void set_pins_for_motor(int index)
{
	int i;
	for (i = 0; i < 4; i++)
	{
		gpio_set_value(gpios[i].gpio, g_motor_pin_ctrl[index] & (1<<i) ? 1 : 0);
	}
}

void disable_motor(void)
{
	int i;
	for (i = 0; i < 4; i++)
	{
		gpio_set_value(gpios[i].gpio, 0);
	}
}

/* int buf[2];
 * buf[0] = 步进的次数, > 0 : 逆时针步进; < 0 : 顺时针步进
 * buf[1] = mdelay的时间
 */
static ssize_t motor_write(struct file *file, const char __user *buf, size_t size, loff_t *offset)
{
    int ker_buf[2];
    int err;
	int step;


    if (size != 8)
        return -EINVAL;

    err = copy_from_user(ker_buf, buf, size);


	if (ker_buf[0] > 0)
	{
		/* 逆时针旋转 */
		for (step = 0; step < ker_buf[0]; step++)
{
			set_pins_for_motor(g_motor_index);
			mdelay(ker_buf[1]);
			g_motor_index--;
			if (g_motor_index == -1)
				g_motor_index = 7;

}
	}
	else
	{
		/* 顺时针旋转 */
		ker_buf[0] = 0 - ker_buf[0];
		for (step = 0; step < ker_buf[0]; step++)
{
			set_pins_for_motor(g_motor_index);
			mdelay(ker_buf[1]);
			g_motor_index++;
			if (g_motor_index == 8)
				g_motor_index = 0;

		}
}

	/* 改进：旋转到位后让马达不再消耗电源 */
	disable_motor();

    return 8;    
}



/* 定义自己的file_operations结构体                                              */
static struct file_operations gpio_key_drv = {
	.owner	 = THIS_MODULE,
	.write   = motor_write,
};

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
		err = gpio_request(gpios[i].gpio, gpios[i].name);
		gpio_direction_output(gpios[i].gpio, 0);
	}

	/* 注册file_operations 	*/
	major = register_chrdev(0, "100ask_gpio_key", &gpio_key_drv);  /* /dev/gpio_desc */

	gpio_class = class_create(THIS_MODULE, "100ask_gpio_key_class");
	if (IS_ERR(gpio_class)) {
		printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
		unregister_chrdev(major, "100ask_gpio_key");
		return PTR_ERR(gpio_class);
	}

	device_create(gpio_class, NULL, MKDEV(major, 0), NULL, "motor"); /* /dev/motor */
	
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
	unregister_chrdev(major, "100ask_gpio_key");

	for (i = 0; i < count; i++)
	{
		gpio_free(gpios[i].gpio);
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


