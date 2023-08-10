/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>

int main(void)
{
    int cnt = 0;
    printk("Hello World! %s", CONFIG_BOARD);
    while (1) {
        printk("cnt:%d", cnt++);
        k_msleep(200);
    }
    return 0;
}
// west build -p always hello_world