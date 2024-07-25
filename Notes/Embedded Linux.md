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

## ErrCode（errno）

| 宏名称          | 键值 | 描述                             |
| --------------- | ---- | -------------------------------- |
| EPERM           | 1    | 不允许的操作                     |
| ENOENT          | 2    | 没有那个文件或目录               |
| ESRCH           | 3    | 没有那个进程                     |
| EINTR           | 4    | 被中断的系统调用                 |
| EIO             | 5    | 输入/输出错误                    |
| ENXIO           | 6    | 没有那个设备或地址               |
| E2BIG           | 7    | 参数列表过长                     |
| ENOEXEC         | 8    | 可执行文件格式错误               |
| EBADF           | 9    | 错误的文件描述符                 |
| ECHILD          | 10   | 没有子进程                       |
| EAGAIN          | 11   | 资源暂时不可用                   |
| EWOULDBLOCK     | 11   | 资源暂时不可用                   |
| ENOMEM          | 12   | 无法分配内存                     |
| EACCES          | 13   | 权限不够                         |
| EFAULT          | 14   | 错误的地址                       |
| ENOTBLK         | 15   | 需要块设备                       |
| EBUSY           | 16   | 设备或资源忙                     |
| EEXIST          | 17   | 文件已存在                       |
| EXDEV           | 18   | 无效的跨设备链接                 |
| ENODEV          | 19   | 没有那个设备                     |
| ENOTDIR         | 20   | 不是目录                         |
| EISDIR          | 21   | 是一个目录                       |
| EINVAL          | 22   | 无效的参数                       |
| ENFILE          | 23   | 系统中打开的文件过多             |
| EMFILE          | 24   | 打开的文件过多                   |
| ENOTTY          | 25   | 对设备不适当的 ioctl 操作        |
| ETXTBSY         | 26   | 文本文件忙                       |
| EFBIG           | 27   | 文件过大                         |
| ENOSPC          | 28   | 设备上没有空间                   |
| ESPIPE          | 29   | 非法 seek 操作                   |
| EROFS           | 30   | 只读文件系统                     |
| EMLINK          | 31   | 过多的链接                       |
| EPIPE           | 32   | 断开的管道                       |
| EDOM            | 33   | 数值参数超出域                   |
| ERANGE          | 34   | 数值结果超出范围                 |
| EDEADLK         | 35   | 已避免资源死锁                   |
| EDEADLOCK       | 35   | 已避免资源死锁                   |
| ENAMETOOLONG    | 36   | 文件名过长                       |
| ENOLCK          | 37   | 没有可用的锁                     |
| ENOSYS          | 38   | 函数未实现                       |
| ENOTEMPTY       | 39   | 目录非空                         |
| ELOOP           | 40   | 符号连接的层数过多               |
| ENOMSG          | 42   | 没有符合需求格式的消息           |
| EIDRM           | 43   | 标识符已删除                     |
| ECHRNG          | 44   | 通道编号超出范围                 |
| EL2NSYNC        | 45   | 级别 2 尚未同步                  |
| EL3HLT          | 46   | 级别 3 已关闭                    |
| EL3RST          | 47   | 级别 3 已重置                    |
| ELNRNG          | 48   | 链接数超出范围                   |
| EUNATCH         | 49   | 未加载协议驱动程序               |
| ENOCSI          | 50   | 没有可用的 CSI 结构              |
| EL2HLT          | 51   | 级别 2 己关闭                    |
| EBADE           | 52   | 无效的交换                       |
| EBADR           | 53   | 无效的请求描述符                 |
| EXFULL          | 54   | 交换满                           |
| ENOANO          | 55   | 没有 anode                       |
| EBADRQC         | 56   | 无效的请求码                     |
| EBADSLT         | 57   | 不适用的 slot                    |
| EBFONT          | 59   | 错误的字体文件格式               |
| ENOSTR          | 60   | 设备不是流                       |
| ENODATA         | 61   | 没有可用的数据                   |
| ETIME           | 62   | 计时器超时                       |
| ENOSR           | 63   | 流资源不足                       |
| ENONET          | 64   | 机器不在网络中                   |
| ENOPKG          | 65   | 包未安装                         |
| EREMOTE         | 66   | 对象是远程的                     |
| ENOLINK         | 67   | 链接已有服务                     |
| EADV            | 68   | 通知错误                         |
| ESRMNT          | 69   | Srmount 错误                     |
| ECOMM           | 70   | 发送时出现通讯错误               |
| EPROTO          | 71   | 协议错误                         |
| EMULTIHOP       | 72   | 尝试 Multihop                    |
| EDOTDOT         | 73   | RFS 特定错误                     |
| EBADMSG         | 74   | 错误的消息                       |
| EOVERFLOW       | 75   | 对已定义的数据类型来说值过大     |
| ENOTUNIQ        | 76   | 网络上的名称不是唯一的           |
| EBADFD          | 77   | 文件描述符处于错误状态           |
| EREMCHG         | 78   | 远程地址已改变                   |
| ELIBACC         | 79   | 无法访问必须的共享库             |
| ELIBBAD         | 80   | 正在访问一个已毁坏的共享库       |
| ELIBSCN         | 81   | a.out 中的 .lib 节已毁坏         |
| ELIBMAX         | 82   | 试图与过多的共享库相链接         |
| ELIBEXEC        | 83   | 无法直接执行共享库               |
| EILSEQ          | 84   | 无效或不完整的多字节字符或宽字符 |
| ERESTART        | 85   | 被中断的系统调用应该重新启动     |
| ESTRPIPE        | 86   | 流管道错误                       |
| EUSERS          | 87   | 用户过多                         |
| ENOTSOCK        | 88   | 对非套接字进行套接字操作         |
| EDESTADDRREQ    | 89   | 需要目标地址                     |
| EMSGSIZE        | 90   | 消息过长                         |
| EPROTOTYPE      | 91   | 错误的 socket 协议类型           |
| ENOPROTOOPT     | 92   | 不可用的协议                     |
| EPROTONOSUPPORT | 93   | 不支持的协议                     |
| ESOCKTNOSUPPORT | 94   | 不支持的套接字类型               |
| EOPNOTSUPP      | 95   | 不支持的操作                     |
| ENOTSUP         | 95   | 不支持的操作                     |
| EPFNOSUPPORT    | 96   | 不支持的协议族                   |
| EAFNOSUPPORT    | 97   | 协议不支持的地址族               |
| EADDRINUSE      | 98   | 地址已在使用                     |
| EADDRNOTAVAIL   | 99   | 无法指定被请求的地址             |
| ENETDOWN        | 100  | 网络已关闭                       |
| ENETUNREACH     | 101  | 网络不可达                       |
| ENETRESET       | 102  | 重置时断开网络连接               |
| ECONNABORTED    | 103  | 由软件导致的连接断开             |
| ECONNRESET      | 104  | 连接被对方重设                   |
| ENOBUFS         | 105  | 没有可用的缓冲区空间             |
| EISCONN         | 106  | 传输端点已连接                   |
| ENOTCONN        | 107  | 传输端点尚未连接                 |
| ESHUTDOWN       | 108  | 无法在传输端点关闭以后发送       |
| ETOOMANYREFS    | 109  | 过多的引用：无法接合             |
| ETIMEDOUT       | 110  | 连接超时                         |
| ECONNREFUSED    | 111  | 拒绝连接                         |
| EHOSTDOWN       | 112  | 主机关闭                         |
| EHOSTUNREACH    | 113  | 没有到主机的路由                 |
| EALREADY        | 114  | 操作已经在进行                   |
| EINPROGRESS     | 115  | 操作现在正在进行                 |
| ESTALE          | 116  | 过旧的文件控柄                   |
| EUCLEAN         | 117  | 结构需要清理                     |
| ENOTNAM         | 118  | 不是 XENIX 命名的类型文件        |
| ENAVAIL         | 119  | 没有可用的 XENIX 信号量          |
| EISNAM          | 120  | 是一个有名类型文件               |
| EREMOTEIO       | 121  | 远程 I/O 错误                    |
| EDQUOT          | 122  | 超出磁盘限额                     |
| ENOMEDIUM       | 123  | 找不到介质                       |
| EMEDIUMTYPE     | 124  | 错误的介质类型                   |
| ECANCELED       | 125  | 操作已取消                       |
| ENOKEY          | 126  | 需要的关键字不存在               |
| EKEYEXPIRED     | 127  | 关键字已过期                     |
| EKEYREVOKED     | 128  | 键值已取消                       |
| EKEYREJECTED    | 129  | 键值被服务所拒绝                 |
| EOWNERDEAD      | 130  | 拥有者已消逝                     |
| ENOTRECOVERABLE | 131  | 状态无法回复                     |
| ERFKILL         | 132  | 由于 RF-kill 而无法操作          |
| EHWPOISON       | 133  | 内存分页有硬件错误               |

除此之外，可以通过`errno`来查询。

```bash
sudo apt install moreutils 
errno -ls
```



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

