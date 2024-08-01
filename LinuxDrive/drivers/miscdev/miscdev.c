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
#include <linux/miscdevice.h>
#include <linux/gpio.h>


struct miscdev_data {
    void *data;
    struct device dev;
    struct miscdevice miscdev;
};


static int miscdev_open(struct inode *inode, struct file *file)
{
    return 0;
}

static long miscdev_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    int ret = -EINVAL;
    struct miscdev_data *data = container_of(filp->private_data, struct miscdev_data, miscdev);

    // use data

    return 0;
}

static int miscdev_release(struct inode *inode, struct file *file)
{
    return 0;
}

static const struct file_operations miscdev_fops = {
    .owner = THIS_MODULE,
    .open = miscdev_open,
    .unlocked_ioctl = miscdev_ioctl,
    .release = miscdev_release,
};

struct miscdev_data *misc_data = NULL;

static int __init miscdev_demo_init(void)
{
    int ret = -1;
    misc_data = kzalloc(sizeof(struct miscdev_data), GFP_KERNEL);
    if (IS_ERR(misc_data)) {
        return -ENOMEM;
    }
    misc_data->miscdev.minor = MISC_DYNAMIC_MINOR;
    misc_data->miscdev.name = "miscdev_demo";
    misc_data->miscdev.fops = &miscdev_fops;
    ret = misc_register(&misc_data->miscdev);
    if (ret) {
        kfree(misc_data);
        return ret;
    }
    return ret;
}

static void __exit miscdev_demo_exit(void)
{
    misc_deregister(&misc_data->miscdev);
    kfree(misc_data);
}

module_init(miscdev_demo_init);
module_exit(miscdev_demo_exit);
MODULE_LICENSE("GPL");