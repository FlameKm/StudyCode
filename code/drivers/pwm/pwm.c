#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/pwm.h>

#define PWMLED_MAX_BRIGHTNESS 1000

typedef enum
{
    PWMLED_CMD_SET_BRIGHTNESS = 0x1,
    PWMLED_CMD_GET_BRIGHTNESS,
} pwmled_cmd_t;

#define PWMLED_PERIOD 1000000// 脉冲周期固定为1ms

static struct {
    struct pwm_device *pwm;
    unsigned int brightness;
} pwmled;

long pwmled_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    printk("pwm set to %d\n", (int)arg);
    switch (cmd) {
        case PWMLED_CMD_SET_BRIGHTNESS:
            // 所谓调节亮度，就是配置占空比，然后使能pwm0
            pwmled.brightness = arg < PWMLED_MAX_BRIGHTNESS ? arg : PWMLED_MAX_BRIGHTNESS;
            pwm_config(pwmled.pwm, pwmled.brightness * 1000, PWMLED_PERIOD);
            if (pwmled.brightness > 0) {
                pwm_enable(pwmled.pwm);
            }
            else {
                pwm_disable(pwmled.pwm);
            }
            break;
        case PWMLED_CMD_GET_BRIGHTNESS:
            return pwmled.brightness;
        default:
            return -EINVAL;
    }
    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = pwmled_ioctl,
};

static struct miscdevice dev = {
    .minor = 0,
    .name = "pwmled",
    .fops = &fops,
    .nodename = "pwmled",
    .mode = 0666,
};

int __init pwmled_init(void)
{
    // 请求PWM0通道
    struct pwm_device *pwm = pwm_request(2, "pwm0");
    if (IS_ERR_OR_NULL(pwm)) {
        printk(KERN_ERR "failed to request pwm\n");
        return PTR_ERR(pwm);
    }

    pwmled.pwm = pwm;
    pwmled.brightness = 0;

    misc_register(&dev);

    return 0;
}

void __exit pwmled_exit(void)
{
    misc_deregister(&dev);
    // 停止并释放PWM0通道
    pwm_disable(pwmled.pwm);
    pwm_free(pwmled.pwm);
}

module_init(pwmled_init);
module_exit(pwmled_exit);

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Phlon | https://ixx.life");
