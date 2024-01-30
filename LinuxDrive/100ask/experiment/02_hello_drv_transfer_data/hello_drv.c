#include "asm/cacheflush.h"
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/mman.h>
#include <linux/random.h>
#include <linux/init.h>
#include <linux/raw.h>
#include <linux/tty.h>
#include <linux/capability.h>
#include <linux/ptrace.h>
#include <linux/device.h>
#include <linux/highmem.h>
#include <linux/backing-dev.h>
#include <linux/shmem_fs.h>
#include <linux/splice.h>
#include <linux/pfn.h>
#include <linux/export.h>
#include <linux/io.h>
#include <linux/uio.h>

#include <linux/uaccess.h>


static struct class *hello_class;
static int major;
static unsigned char hello_buf[100];

static int hello_open (struct inode *node, struct file *filp)
{
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    return 0;
}
static ssize_t hello_read (struct file *filp, char __user *buf, size_t size, loff_t *offset)
{
    unsigned long len = size > 100 ? 100 : size;

    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);

    copy_to_user(buf, hello_buf, len);

    return len;
}

static ssize_t hello_write(struct file *filp, const char __user *buf, size_t size, loff_t *offset)
{
    unsigned long len = size > 100 ? 100 : size;

    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    copy_from_user(hello_buf, buf, len);

    return len;
}

static int hello_release (struct inode *node, struct file *filp)
{
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    return 0;
}

/* 1. create file_operations */
static const struct file_operations hello_drv = {
    .owner      = THIS_MODULE,
	.read		= hello_read,
	.write		= hello_write,
	.open		= hello_open,
    .release    = hello_release,
};


/* 2. register_chrdev */

/* 3. entry function */
static int hello_init(void)
{
    major = register_chrdev(0, "100ask_hello", &hello_drv);

	hello_class = class_create(THIS_MODULE, "hello_class");
	if (IS_ERR(hello_class)) {
		printk("failed to allocate class\n");
		return PTR_ERR(hello_class);
	}

    device_create(hello_class, NULL, MKDEV(major, 0), NULL, "hello");  /* /dev/hello */

   return 0;
}


/* 4. exit function */
static void hello_exit(void)
{
    device_destroy(hello_class, MKDEV(major, 0));

    class_destroy(hello_class);

    unregister_chrdev(major, "100ask_hello");
}


module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
