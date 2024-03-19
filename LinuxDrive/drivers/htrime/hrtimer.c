/*
 * @Author       : ChenshengyangPointspread chenshengyang@pointspread.cn
 * @Date         : 2023-05-30 10:41:44
 * @LastEditors: ChenshengyangPointspread chenshengyang@pointspread.cn
 * @LastEditTime: 2023-05-31 16:51:48
 * @FilePath     : /hrtimer-demo/hrtimer_demo.c
 * @Description  : hrtiemr_demo
 * 
 * Copyright (c) 2023 by ChenshengyangPointspread chenshengyang@pointspread.cn, All Rights Reserved. 
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/hrtimer.h>
#include <linux/jiffies.h>

#include <linux/gpio.h>

enum { GPIO_LOW = 0, GPIO_HIGH = 1 };
#define GPIO_NUM 28

//定义一个hrtimer
static struct hrtimer timer;
ktime_t kt;
 
//定时器回调函数
static enum hrtimer_restart hrtimer_hander(struct hrtimer *timer)
{
    // printk("GPIO_%d is_on: %d\r\n", GPIO_NUM, gpio_get_value(GPIO_NUM));
    gpio_set_value(GPIO_NUM, !(gpio_get_value(GPIO_NUM)));
    hrtimer_forward(timer,timer->base->get_time(),kt);//hrtimer_forward(timer, now, tick_period);
    return HRTIMER_RESTART;  //重启定时器
}
 
static int __init hrtimer_demo_init(void)
{
    printk("---------%s-----------\r\n",__func__);
    gpio_request(GPIO_NUM, "GPIO");
    gpio_direction_output(GPIO_NUM, GPIO_LOW);
    kt = ktime_set(0,25000);// 0s  5000  = 5us 定时
    hrtimer_init(&timer,CLOCK_REALTIME,HRTIMER_MODE_ABS);
    hrtimer_start(&timer,kt,HRTIMER_MODE_ABS);
    timer.function = hrtimer_hander;
    return 0;
}
 
static void __exit hrtimer_demo_exit(void)
{
    hrtimer_cancel(&timer);
    printk("------------test over---------------\r\n");
}

module_init(hrtimer_demo_init);
module_exit(hrtimer_demo_exit);
MODULE_LICENSE("GPL");