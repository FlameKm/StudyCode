#include "zephyr/sys/printk.h"
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>


static const struct gpio_dt_spec key = GPIO_DT_SPEC_GET(DT_ALIAS(key0), gpios);

int main(void)
{
    int ret;

    if (!gpio_is_ready_dt(&key)) {
        return 0;
    }

    ret = gpio_pin_configure_dt(&key, GPIO_INPUT);
    if (ret < 0) {
        return 0;
    }
    printk("hello world");
    while (1) {
        ret = gpio_pin_get_dt(&key);
        printk("key is %d\n", ret);
        k_msleep(200);
    }
    return 0;
}
