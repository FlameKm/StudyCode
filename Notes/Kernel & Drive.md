# Kernel & Drives

## 调试

### KGDB

1. 内核配置中打开KGDB
2. 启动参数添加`kgdb=ttyS0,115200`(举例)，`waitgdb`
3. 使用交叉工具链的gdb或者gdb-multiarch，加载vmlinux，连接内核。

**QEMU**

qemu启动参数添加`-s -S`

### GDB

1. 对内核进行调试

2. 加载符号表，使用下面脚本 ，在被调试端执行 `sudo ko-to-symbols.sh usb_f_uvc.ko`

   其中，sudo ko-to-symbols.sh 就是下面脚本。

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

3. 将得到的信息全部复制到主机gdb面板中，即可调试gdb

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

编译内核选项中，`-m`表示动态模块，`-y`表示编译进内核

# System

## **GPIO子系统**

### 查看gpio使用状态

```shell
cat /sys/kernel/debug/gpio
```

### 确定GPIO引脚的编号

① 先在开发板的`/sys/class/gpio`目录下，找到各个`gpiochipXXX`目录：

![](/home/hyc/Project/StudyCode/Notes/media/image33.png)

② 然后进入某个`gpiochipXXX`目录，查看文件`label`的内容，就可以知道起始号码XXX对于哪组GPIO

### shell控制

以引脚编号为110为例.

```shell
echo 110 > /sys/class/gpio/export ## gpio_request
echo in > /sys/class/gpio/gpio110/direction ## gpio_direction_input
cat /sys/class/gpio/gpio110/value ## gpio_get_value
echo 110 > /sys/class/gpio/unexport ## gpio_free
```

对于输出,以N为例

```shell
echo 104> /sys/class/gpio/export
echo out > /sys/class/gpio/gpio104/direction
echo 1 > /sys/class/gpio/gpio104/value
echo 104> /sys/class/gpio/unexport
```

### GPIO子系统函数

| **descriptor-based**       | **legacy**            |
| -------------------------- | --------------------- |
| **获得GPIO**               |                       |
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
| **释放GPIO**               |                       |
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

gpio子系统中：

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

新内核6.x.x

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
#include <linux/time.h>
...
uint64_t start,end;

start = ktime_get_ns();
// do something
end = ktime_get_ns();
printf("hyc xxx test: %d", end - start);
```

还有一些封装好的api直接获取time

`ktime_get_ts`

`ktime_get_ns`
