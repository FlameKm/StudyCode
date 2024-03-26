/* These code snippets are just a demonstration */

#define DEVICE_NAM "xxx"


static int xxx_open(struct inode *inode, struct file *file)
{
    return 0;
}

static long xxx_ioctl(struct file *fd, unsigned int cmd, unsigned long arg)
{
    return 0;
}

static const struct file_operations xxx_fops = {
    .owner = THIS_MODULE,
    .open = xxx_open,
    .unlocked_ioctl = xxx_ioctl,
};

struct ps_xxx_dev {
    dev_t dev;
    struct cdev cdev;
    struct class *class;
    struct device *device;
};

static int xxx_chrdev_init(struct i2c_client *client, struct ps_xxx_dev *dev)
{
    int ret = 0;
    ret = alloc_chrdev_region(&dev->dev, 0, 1, DEVICE_NAM);
    if (ret < 0) {
        pr_err("Failed to allocate device numbers\n");
        goto err0;
    }
    cdev_init(&dev->cdev, &xxx_fops);
    ret = cdev_add(&dev->cdev, dev->dev, 1);
    if (ret < 0) {
        pr_err("Failed to add char device\n");
        goto err1;
    }
    dev->class = class_create(THIS_MODULE, DEVICE_NAM);
    if (IS_ERR(dev->class)) {
        pr_err("Failed to create device class\n");
        goto err2;
    }
    dev->device = device_create(dev->class, NULL, dev->dev, NULL, DEVICE_NAM);
    if (IS_ERR(dev->device)) {
        pr_err("Failed to create device\n");
        goto err3;
    }
    return ret;

err3:
    class_destroy(dev->class);
err2:
    cdev_del(&dev->cdev);
err1:
    unregister_chrdev_region(dev->dev, 1);
err0:
    return -1;
}

static int xxx_chrdev_deinit(struct i2c_client *client)
{
    int ret = 0;
    struct ps_xxx_dev *dev = i2c_get_clientdata(client);
    device_destroy(dev->class, dev->dev);
    class_destroy(dev->class);
    cdev_del(&dev->cdev);
    unregister_chrdev_region(dev->dev, 1);
    return ret;
}

xxx_chrdev_deinit(struct i2c_client *client)
{
    int ret = 0;
    struct ps_xxx_dev *dev = i2c_get_clientdata(client);
    device_destroy(dev->class, dev->dev);
    class_destroy(dev->class);
    cdev_del(&dev->cdev);
    unregister_chrdev_region(dev->dev, 1);
    return ret;
}

static int xxx_probe(struct i2c_client *client, const struct i2c_device_id *device_id)
{
    int ret = 0;
    struct ps_xxx_dev *priv;

    priv = devm_kzalloc(&client->dev, sizeof(struct ps_xxx_dev), GFP_KERNEL);
    if (IS_ERR(priv)) {
        goto err;
    }
    i2c_set_clientdata(client, priv);

    ret = xxx_chrdev_init(client, priv);
    if (ret < 0) {
        pr_err(" Failed to chrdev init");
        goto err1;
    }
    return ret;

err1:
err:
    devm_kfree(&client->dev, priv);
}

static const struct of_device_id xxx_of_match_table[] = {
    {.compatible = "ps,xxx"},
    {},
};

module_i2c_driver(xxx_driver);
MODULE_LICENSE("GPL");