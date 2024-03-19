# Drives Code

## **GPIO**

### GPIO Status

```shell
cat /sys/kernel/debug/gpio
```

### Get GPIO ID

① 先在开发板的 `/sys/class/gpio` 目录下，找到各个 `gpiochipXXX` 目录：

![](/home/hyc/Project/StudyCode/Notes/media/image33.png)

② 然后进入某个 `gpiochipXXX` 目录，查看文件 `label` 的内容，就可以知道起始号码 XXX 对于哪组 GPIO

### Use GPIO In Shell

这个是基于标准的 linux 内核

以引脚编号为 110 为例.

```shell
echo 110 > /sys/class/gpio/export ## gpio_request
echo in > /sys/class/gpio/gpio110/direction ## gpio_direction_input
cat /sys/class/gpio/gpio110/value ## gpio_get_value
echo 110 > /sys/class/gpio/unexport ## gpio_free
```

对于输出, 以 N 为例

```shell
echo 104> /sys/class/gpio/export
echo out > /sys/class/gpio/gpio104/direction
echo 1 > /sys/class/gpio/gpio104/value
echo 104> /sys/class/gpio/unexport
```

### GPIO Function

| **descriptor-based**       | **legacy**            |
| -------------------------- | --------------------- |
| **获得 GPIO**              |                       |
| **gpiod_get**              | gpio_request          |
| **gpiod_get_index**        |                       |
| **gpiod_get_array**        | gpio_request_array    |
| **devm_gpiod_get**         |                       |
| **devm_gpiod_get_index**   |                       |
| **devm_gpiod_get_array**   |                       |
| **设置方向**               |                       |
| **gpiod_direction_input**  | gpio_direction_input  |
| **gpiod_direction_output** | gpio_direction_output |
| **读值、写值**             |                       |
| **gpiod_get_value**        | gpio_get_value        |
| **gpiod_set_value**        | gpio_set_value        |
| **释放 GPIO**              |                       |
| **gpio_free**              | gpio_free             |
| **gpiod_put**              | gpio_free_array       |
| **gpiod_put_array**        |                       |
| **devm_gpiod_put**         |                       |
| **devm_gpiod_put_array**   |                       |

## I2C Subsystem

### base

**独立模块**

`module_i2c_driver` 进行 init 和 exit

**注册**

一般使用这两个函数，本质上是一样的，第二个就是对本模块的宏

`int i2c_register_driver(struct module *owner,struct i2c_driver *driver)` 

`i2c_add_driver(driver)`

**卸载**

`void i2c_del_driver(struct i2c_driver *driver);`

### Demo

[I2CDemo](../LinuxDrive/drive/iic)

### Data

由于接口多数是 `struct i2c_client *client`，没有直接的数据传输，因此驱动有许多数据保存为单个模块的方式

client 结构中有一个成员就是 dev, 可以使用 `of_device_get_match_data` 获取，接着使用 `dev_set_drvdata`， `dev_get_drvdata`

也可以使用已经封装好的 `i2c_set_clientdata`，`i2c_get_clientdata`

还有 dev 转换成 client，`to_i2c_client`

### Transfer

- int i2c_master_send(const struct i2c_client *client, const char * buf, int count)

- int i2c_master_recv(const struct i2c_client *client, char * buf, int count)
- int i2c_transfer(struct i2c_adapter *adap, struct i2c_msg * msgs, int num)

详见 demo，前两个发送都是对第三个的一个封装。发送的时序是：

`start` -> `device addr` -> `data(contain reg)` -> `end`

## POLL

参考链接：https://cloud.tencent.com/developer/article/1708996

实现一定时间内等待响应。

驱动里一般做两件事

1. 把当前线程挂如队列，poll_wait

APP 调用一次 poll，可能导致 drv_poll 被调用 2 次，但是我们并不需要把当前线程挂入队列 2 次。 可以使用内核的函数 poll_wait 把线程挂入队列，如果线程已经在队列里了，它就不会再次挂入。

2. 返回设备状态

APP 调用 poll 函数时，有可能是查询“有没有数据可以读”：POLLIN，也有可能是查询“你有没有空间给我写数据”：POLLOUT。 所以 drv_poll 要返回自己的当前状态：(POLLIN | POLLRDNORM) 或 (POLLOUT | POLLWRNORM)。

应用程序一般调用 poll、select、epoll。

在调用 poll 函数时，要指明监测的文件、判断的状态。

```c
struct pollfd fds[1];
int timeout_ms = 5000;
int ret;

fds[0].fd = fd;
fds[0].events = POLLIN;

ret = poll(fds, 1, timeout_ms);
if ((ret == 1) && (fds[0].revents & POLLIN))
{
    read(fd, &val, 4);
    printf("get button : 0x%x\n", val);
}
```

调用 select 时

```c
fd_set rdfds;   
struct timeval tv;
int rtn = 0;

FD_ZERO(&rdfds);     //清空 fd_set
FD_SET(fd, &rdfds);  //将要检测的 fd 描述符加入到 fd_set 集合中

tv.tv_sec = 2;
tv.tv_usec = 1000;      //设置超时时间为 2s+1ms

rtn = select(fd+1, &rdfds, NULL, NULL, &tv);

if(rtn < 0)
    perror("select");
else if(0 == rtn)
    printf("timeout\n");
else
{
    printf("rtn = %d\n", rtn);      //查看有多少个文件描述符发生了变化

    if(FD_ISSET(socket, &rdfds))    //判断下这个 socket 是否状态真的变成了可读
    {
        recv(...);
    }
}
```



## EVENT

| POLLIN     | 有数据可读                                                |
|------------|------------------------------------------------------|
| POLLRDNORM | 等同于 POLLIN                                           |
| POLLRDBAND | Priority band data can be read，有优先级较较高的“band data”可读 |
|            | Linux 系统中很少使用这个事件                                    |
| POLLPRI    | 高优先级数据可读                                             |
| POLLOUT    | 可以写数据                                                |
| POLLWRNORM | 等同于 POLLOUT                                          |
| POLLWRBAND | Priority data may be written                         |
| POLLERR    | 发生了错误                                                |
| POLLHUP    | 挂起                                                   |
| POLLNVAL   | 无效的请求，一般是 fd 未 open                                  |

## IRQ

### Basis

比如 KEY 驱动程序

```C
static irqreturn_t xxx_isr(int irq, void *data)
{
    printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

    return IRQ_HANDLED;
}

// 获取中断号
irq = gpio_to_irq(temp_measure->gpio);
// 注册中断
ret = request_irq(irq, xxx_isr, IRQF_TRIGGER_RISING, "key_irq", &key_irq[0]); 
// 释放中断
free_irq(irq, &key_irq[0]);
```

中断函数返回值的含义

| IRQ RETURN      | MEANING                                               |
| --------------- | ----------------------------------------------------- |
| IRQ_NONE        | interrupt was not from this device or was not handled |
| IRQ_HANDLED     | interrupt was handled by this device                  |
| IRQ_WAKE_THREAD | handler requests to wake the handler thread           |

触发条件

| TRIG SIGNAL          | MEANING                              |
|----------------------|--------------------------------------|
| IRQF_TRIGGER_NONE    | 无触发条件                                |
| IRQF_TRIGGER_RISING  | 上升沿触发                                |
| IRQF_TRIGGER_FALLING | 下降沿触发                                |
| IRQF_TRIGGER_HIGH    | 高电平触发，表示中断在输入信号为高电平时触发。              |
| IRQF_TRIGGER_LOW     | 低电平触发，表示中断在输入信号为低电平时触发。              |
| IRQF_TRIGGER_MASK    | 用于屏蔽中断触发方式的位掩码。                      |
| IRQF_TRIGGER_PROBE   | 用于探测中断触发方式，通常在注册中断时使用，让系统尝试检测中断触发方式。 |

### Interrupts Status

`cat /proc/interrupts`

### Tasklet

是一个可以在由系统决定的安全时刻在中断上下文中运行的函数，可能会被多次调用，但是调度不会积累，实际只会运行一次。可以较好的解决 **中断底半部**，除了 tasklet 实现底半部的方式之外，还可以使用软中断、工作队列等来实现底半部，一般来说，**十分耗时的任务使用内核线程来实现**。

可以使用宏 `DECLARE_TASKLET` 进行声明 taklet。

Demo

```c
void xxx_do_tasklet(unsigned long);
DECLARE_TASKLET(xxx_tasklet, xxx_do_tasklet, 0);

// Bottom
static void xxx_do_tasklet(unsigned long){
	...
}

// Top
static irqreturn_t xxx_isr(int irq, void *dev_id)
{
    ...
    tasklet_schedule(&xxx_tasklet);
    ...
    return IRQ_HANDLED;
}

// Init
tasklet_init(&xxx_tasklet, xxx_do_tasklet, &dev);

// Remove
tasklet_kill(&xxx_tasklet);
```

### Disable IRQs

禁用全部中断

```c
void local_irq_save(unsigned long flags);
void local_irq_disable(void);
```

恢复全部中断

```c
void local_irq_restore(unsigned long flags);
void local_irq_enable(void);
```

指定中断线

```c
void disable_irq(unsigned int irq);
void disable_irq_nosync(unsigned int irq);
void enable_irq(unsigned int irq);
void synchronize_irq(unsigned int irq);
```

### Kernel Thread

在中断下半部时，如果处理起来十分耗时，那么可以使用 work 来实现。本质是上半部中创建工作队列

```c
int request_threaded_irq(unsigned int irq, irq_handler_t handler,
			 irq_handler_t thread_fn, unsigned long irqflags,
			 const char *devname, void *dev_id)
```

## Work Queue

以下 API 为 2.6.20 之后的内核

### Work

```c
// init
struct work_struct work;
INIT_WORK(&work, func);

// schedulezhong duan
schedule_work(&work);

// other
flush_work(&xx_dwork);
cancel_work_sync(&xx_dwork); // use to exit
```

### Delay Work

```C
// init
struct delayed_work xx_dwork;
INIT_DELAYED_WORK(&xx_dwork, func); 

// schedule
schedule_delayed_work(&dwork, delay); //delay number of jiffies

// other
flush_delayed_work(&xx_dwork);
cancel_delayed_work(&xx_dwork); // use to exit
```



## Counting Time

### Count

新内核 6.x.x

```c
#include <linux/time.h>
#include <linux/timekeeping.h>
...
struct timespec64 ts_start, ts_end, ts_delta;

ktime_get_boottime_ts64(&ts_start);
// do something
ktime_get_boottime_ts64(&ts_end);
ts_delta = timespec64_sub(ts_end,ts_start);
printk("hyc xxx test: %d", timespec64_to_ns(&ts_delta));
```

新旧内核

```bash
nclude <linux/time.h>
...
uint64_t start,end;

start = ktime_get_ns();
// do something
end = ktime_get_ns();
printf("hyc xxx test: %d", end - start);
```

还有一些封装好的 api 直接获取 time

`ktime_get_ts`

`ktime_get_ns`

### Jiffies

 jiffies 记录系统时钟中断次数，即 tick 的次数，初始值并不是 0

相关函数

```c
extern unsigned int jiffies_to_msecs(const unsigned long j);
extern unsigned int jiffies_to_usecs(const unsigned long j);
extern unsigned long msecs_to_jiffies(const unsigned int m);
extern unsigned long usecs_to_jiffies(const unsigned int u);
extern unsigned long timespec_to_jiffies(const struct timespec *value);
extern void jiffies_to_timespec(const unsigned long jiffies,
                struct timespec *value);
extern unsigned long timeval_to_jiffies(const struct timeval *value);
extern void jiffies_to_timeval(const unsigned long jiffies,
                   struct timeval *value);
extern clock_t jiffies_to_clock_t(unsigned long x);
extern unsigned long clock_t_to_jiffies(unsigned long x);
extern u64 jiffies_64_to_clock_t(u64 x);
extern u64 nsecs_to_jiffies64(u64 n);
extern unsigned long nsecs_to_jiffies(u64 n);
```

## Hrtimer

```c
static enum hrtimer_restart hrtimer_hander(struct hrtimer *timer)
{
    // do something
    return HRTIMER_RESTART; //restart
}

//init 
hrtimer_init(&timer, CLOCK_REALTIME, HRTIMER_MODE_ABS); //系统时钟源，绝对时间模式

//start
hrtimer_start(&timer, kt, HRTIMER_MODE_ABS);

//remove
hrtimer_cancel(&timer);
```

循环模式时候，必须使用 HRTIMER_MODE_ABS

# Appliction Code

## I2C

参考网址

[Linux 应用开发【第十二章】I2C 编程应用开发](https://blog.51cto.com/weidongshan/4795722#1231_I2C_toolsI2C_233)

### API

具体可以查看 <linux/i2c-dev.h>

- ioctl，会发送设备地址，也可以用来发送数据

```c
    ret = ioctl(cam_fd[bus], I2C_SLAVE_FORCE, i2c_addr);
    or
    ret = ioctl(cam_fd[bus], I2C_RDWR, (unsigned long)&data); // data is struct i2c_rdwr_ioctl_data
```

- write，直接发送数据
- read， 直接读取数据



### I2C-Tools

- i2cdetect：用于扫描 i2c 总线上的设备，并显示地址

- i2cset：设置 i2c 设备某个寄存器的值

- i2cget：读取 i2c 设备某个寄存器的值

- i2cdump：读取某个 i2c 设备所有寄存器的值

- i2ctransfer：一次性读写多个字节

# System Operate

## DEBUG

### KGDB

1. 内核配置中打开 KGDB
2. 启动参数添加 `kgdb=ttyS0,115200`(举例)，`waitgdb`
3. 使用交叉工具链的 gdb 或者 gdb-multiarch，加载 vmlinux，连接内核。

**QEMU**

qemu 启动参数添加 `-s -S`

### GDB

1. 对内核进行调试

2. 加载符号表，使用下面脚本 ，在被调试端执行 `sudo ko-to-symbols.sh usb_f_uvc.ko`

   其中，ko-to-symbols.sh 就是下面脚本。

```bash
module_name="$(basename $1 .ko)"
cd /sys/module/$module_name/sections
echo -n add-symbol-file $1 `/bin/cat .text`
for section in .[a-z]* *; do
    if [ $section != ".text" ]; then
echo -n " -s" $section `/bin/cat $section` 
    fi
done
echo

```

3. 将得到的信息全部复制到主机 gdb 面板中，即可调试 gdb

## Change Kernel

### Delect

删除相关文件

```bash
KERNEL_VERSION=6.7.0
sudo rm -rf  /boot/vmlinuz*${KERNEL_VERSION}*
sudo rm -rf  /boot/initrd*${KERNEL_VERSION}*
sudo rm -rf  /boot/System-map*${KERNEL_VERSION}*
sudo rm -rf  /boot/config-*${KERNEL_VERSION}*
sudo rm -rf  /lib/modules/*${KERNEL_VERSION}*/
sudo rm -rf  /var/lib/initramfs-tools/*${KERNEL_VERSION}*/
```

更新配置

```bash
sudo update-grub2
```

### Change

下载内核源码

```bash
apt-get install linux-source-...
cd /lib/modules/$(uname -r)/build
ls
```

有这些项： debian  debian.hwe-6.5  linux-source-6.5.0.tar.bz2

解压内核即可获得内核

编译后替换

```bash
make modules_install # 将编译好的模块文件安装到系统中
make install # 更新内核
```

## Drive Install

安装 `insmod xxx` or  `modprobe xxx`

卸载 `rmmod xxx` or  `modprobe -r xxx`

编译内核选项中，`-m` 表示动态模块，`-y` 表示编译进内核

# Device Tree

## Device Tree Restore

1. 在开发板系统目录/sys/firmware 中有 `fdt` 文件，将它拷贝到 pc 机
2. 使用 kernel 目录下 dtc 工具复原

```bash
cd ${KERNEL_DIR}
scripts/dtc/dtc -I dtb -O dts -o ~/Downloads/fdt.dts ~/Downloads/fdt
```

## Overlay

### latest code

```dts
/dts-v1/;
/plugin/;

&{/} {
    /*此处在根节点"/"下,添加要插入的节点或者属性*/
};

&XXXXX {
    /*此处在节点"XXXXX"下,添加要插入的节点或者属性*/
};
```

### early code

```dts
/dts-v1/;
/plugin/;

 / {
        fragment@0 {
            target-path = "/";
            __overlay__ {
                /*在此添加要插入的节点*/
                .......
            };
        };

        fragment@1 {
            target = <&XXXXX>;
            __overlay__ {
                /*在此添加要插入的节点*/
                .......
            };
        };
    .......
 };
```

还有 `__overlay__`,  `__symbols__`,  `__fixups__`, `__localfixups_`

### compile

使用内核的 dtc 工具

```bash
dtc -I dts -O dtb -o xxx.dtbo xxx.dts
```

## Sign Kernel

[web link](https://blog.csdn.net/qq_40227064/article/details/124467228)

```bash
./scripts/sign-file sha256 certs/signing_key.pem certs/signing_key.x509 yuor_module_path
```

