#include <linux/init.h> 
#include <linux/module.h> 
#include <linux/timer.h>

static void timer_function(unsigned long data);
DEFINE_TIMER(test_timer,timer_function,0,0); 

static void timer_function(unsigned long data)
{
	printk("This is timer function!\n");
	//mod_timer(&test_timer,jiffies + 2*HZ); 
}

static int hello_init(void)
{
	printk("hello world!\n");
	test_timer.expires = jiffies + 1*HZ; //定义好未来时刻的时间点
	add_timer(&test_timer);  //向Linux内核注册定时器
    mod_timer(&test_timer,jiffies + 10*HZ); 
	return 0;
}

static void hello_exit(void)
{
	printk("bye!\n");
	del_timer(&test_timer); //删除定时器
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");