# Kernel & Drives

## 调试

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

## 内核变更

### 删除内核

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

### 替换内核

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

## 驱动变更

安装 `insmod xxx` or  `modprobe xxx`

卸载 `rmmod xxx` or  `modprobe -r xxx`

编译内核选项中，`-m` 表示动态模块，`-y` 表示编译进内核

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

比如KEY驱动程序

```C
static irqreturn_t isr_f(int irq, void *data)
{
    printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

    return IRQ_HANDLED;
}

// 获取中断号
irq = gpio_to_irq(temp_measure->gpio);
// 注册中断
ret = request_irq(irq, isr_f, IRQF_TRIGGER_RISING, "key_irq", &key_irq[0]); 
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

# System

## **GPIO 子系统**

### 查看 gpio 使用状态

```shell
cat /sys/kernel/debug/gpio
```

### 确定 GPIO 引脚的编号

① 先在开发板的 `/sys/class/gpio` 目录下，找到各个 `gpiochipXXX` 目录：

![](/home/hyc/Project/StudyCode/Notes/media/image33.png)

② 然后进入某个 `gpiochipXXX` 目录，查看文件 `label` 的内容，就可以知道起始号码 XXX 对于哪组 GPIO

### shell 控制

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

### GPIO 子系统函数

| **descriptor-based**       | **legacy**            |
|----------------------------|-----------------------|
| **获得 GPIO**                |                       |
| **gpiod_get**              | gpio_request          |
| **gpiod_get_index**        |                       |
| **gpiod_get_array**        | gpio_request_array    |
| **devm_gpiod_get**         |                       |
| **devm_gpiod_get_index**   |                       |
| **devm_gpiod_get_array**   |                       |
| **设置方向**                   |                       |
| **gpiod_direction_input**  | gpio_direction_input  |
| **gpiod_direction_output** | gpio_direction_output |
| **读值、写值**                  |                       |
| **gpiod_get_value**        | gpio_get_value        |
| **gpiod_set_value**        | gpio_set_value        |
| **释放 GPIO**                |                       |
| **gpio_free**              | gpio_free             |
| **gpiod_put**              | gpio_free_array       |
| **gpiod_put_array**        |                       |
| **devm_gpiod_put**         |                       |
| **devm_gpiod_put_array**   |                       |

## 中断

### 流程

在驱动程序里使用中断的流程如下：

1.  确定中断号

注册中断处理函数，函数原型如下：

```c
int request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags,const char *name, void *dev);
```

2.  在中断处理函数里

- 分辨中断

- 处理中断

- 清除中断

### 获取中断号

gpio 子系统中：

```c
int gpio_to_irq(unsigned int gpio);
int gpiod_to_irq(const struct gpio_desc *desc);
```

### 获取中断名称

```shell
cat /proc/interrupts
```

### 触发方式类型

```c
#define IRQF_TRIGGER_NONE 0x00000000
#define IRQF_TRIGGER_RISING 0x00000001
#define IRQF_TRIGGER_FALLING 0x00000002
#define IRQF_TRIGGER_HIGH 0x00000004
#define IRQF_TRIGGER_LOW 0x00000008
#define IRQF_SHARED 0x00000080
```

## 计时

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
