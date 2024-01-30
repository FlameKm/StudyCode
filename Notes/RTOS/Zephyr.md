# Zephyr

## 个人理解

Zephyr 操作系统类似一个单片机开发 IDE，代码具有高耦合度，与硬件高解耦。能够提高代码的复用率，大概的实现方式是，将代码层面使用设备树进行管理硬件，通过 cmake（或者其它）工具将设备树文件转换为一个头文件，其中包含了不同的硬件使用的不同的宏，比如 STM32 中 GPIO 的 port、pin，从而实现代码的解耦。

优点：

- 代码高复用

- 设备树管理

- 开发方式与 linux 较为相似。

缺点：

- 门槛高

- 工具链复杂

## Hello World

### 安装

<https://docs.zephyrproject.org/latest/develop/getting_started/index.html>

#### python 虚拟环境

```shell
装
sudo apt install python3-venv

#创建
python3 -m venv ~/zephyrproject/.venv

#打开
source ~/zephyrproject/.venv/bin/activate

#关闭
deactivate
```



### 编译烧录（支持的开发板）

```shell
cd ~/zephyrproject/zephyr/ #进入工程目录
```

**构建**

```shell
west build -p always -b stm32f103_mini samples/basic/blinky
```

或.

```shell
rm -rf build/* #清除上次构建
cmake -Bbuild -GNinja -DBOARD=stm32f103_mini samples/basic/blinky
ninja -Cbuild
```

cmake 的解释

`-B` 生成目录

`-G` 构建工具

`-DBOARD` 为了确定设备树

`最后一项` 工程目录

**烧录**

```shell
west flash
# or
ninja flash
```

这个是配置在 build.ninja 的脚本，如果使用 make 的话就是 makefile 里



## 调度算法

Zephyr 使用的调度算法主要是多级反馈队列（Multilevel Feedback Queue, MLFQ）调度算法，它是一种基于优先级的动态调度策略。这个算法允许任务在运行时改变其优先级，从而根据任务的行为和特性来动态地调整任务的执行顺序。

高低优先级：**抢占**

同等优先级：**协作式**

## 启动方式

![](media/image62.png)

## 时间

### 定时器

地址：<https://docs.zephyrproject.org/latest/kernel/services/timing/timers.html>

**使用计时器在指定时间后启动异步操作。**

**使用计时器来确定是否已经过了指定的时间。特别是，当需要比更简单的 k_sleep() 和 k_usleep() 调用提供的精度和/或单位控制更高的精度和/或单位控制时，应使用计时器。**

**在执行涉及时间限制的操作时，使用计时器来执行其他工作。**

计时器是一个内核对象，它使用内核的系统时钟来测量时间的流逝。当达到计时器指定的时间限制时，它可以执行应用程序定义的操作，也可以简单地记录到期时间并等待应用程序读取其状态。

- 类型：`k_timer`

- 定义：`k_timer_init` or `K_TIMER_DEFINE`

- 启动：`k_timer_start`

- 停止：`k_timer_stop`

  API

  ![](media/image63.png)

  如果不需要可以指定为 `NULL`

  ![](media/image64.png)

  ![](media/image65.png)

  `duration`: 指定计时器第一次到期之前的时间间隔的持续时间。

  `period`: 指定第一个计时器到期后所有计时器到期之间的时间间隔的周期

  例如，如果计时器以 200 的持续时间和 75 的周期启动，它将首先在 200ms 后过期，然后每 75ms 过期一次。

**举例**：

### 计时器

1.  调用 `timing_init()` 来初始化定时器。

2.  调用 `timing_start()` 以发出开始收集计时信息的信号。这通常会启动计时器。

3.  调用 `timing_counter_get()` 来标记代码执行的开始。

4.  调用 `timing_counter_get()` 来标记代码执行结束。

5.  调用 `timing_cycles_get()` 来获取代码执行开始和结束之间的定时器周期数。

6.  使用总周期数调用 `timing_cycles_to_ns()`，将周期数转换为纳秒。

7.  重复步骤 3 以收集其他代码块的计时信息。

8.  调用 `timing_stop()` 以发出计时信息收集结束的信号。这通常会停止计时器。

### 时间转换

`K_SECONDS`：生成秒级超时延迟

`K_MSEC`：生成毫秒级的超时延迟。

`K_MINUTES`：生成几分钟的超时延迟。

`K_HOURS`：生成几小时的超时延迟。

`K_FOREVER`：没有参数，无限长时间

## 设备树

**总结**

- DeviceTree 本身的语法只提供了一个基于总线主从关系的树形层次结构，此外每个节点可以用属性来存储信息。语法本身并没有规定硬件要如何描述。
- DeviceTree 中的一些常见属性，补充了这方面的空缺。
  - reg、ranges、#address-cells、 #size-cells 这四个属性描述了总线上的地址分配
  - status 属性描述了设备是否使能
  - compatible 属性描述了设备的兼容性
- 在 DeviceTree 中，除了本身的树形结构以外，还具有一些逻辑上的树形结构，称为域。域具有控制器和设备节点，控制器是真正实现域的功能的硬件外设，而设备节点只是为了开发方便解耦而进行的一种抽象。
- 真正限制 device tree 中属性该如何写的，是 device binding 文件。binding 文件是芯片厂商提供的。有了 binding 文件，就可以在 Vs Code 中实现自动的检查与补全。Zephyr 实际构建项目时，也是参考 binding 文件来检查 dts 的正确性。只有 dts 按照正确的规则写了，zephyr 的驱动代码才能识别到硬件配置，进行自动初始化。
- zephyr 中会有一些特殊的虚拟节点来为开发提供便利。

### 节点

要想在代码中访问到 DeviceTree 中的信息，需要通过 `DeviceTree API` 来实现

```c
#include <zephyr/devicetree.h>
```

为了获得某个节点的属性，首先需要这个节点的 id (node identifier)来作为句柄。节点 id 本质上就是 devicetree generated.h 中的宏定义。
获得节点 id 的方式有很多

| 获取方式            | 示例                          | 说明                                                  |
| ------------------- | ----------------------------- | ----------------------------------------------------- |
| 根节点              | DT_ROOT                       | 根节点 id                                              |
| soc_serial_40001000 | DT_PATH(soc, serial_40001000) | /soc/serial@40001000                                  |
| Label               | DT_NODELABEL(serial1)         | 根据 dts 中定义的 label 来找到节点                        |
| chosen 节点          | DT_CHOSEN(zephyr_console)     | 根据 dts 中 chosen 节点的配置: <br /> zephyr, console =&uart0 |

参考地址：https://docs.zephyrproject.org/latest/build/dts/api/api.html#generic-apis

1. **DT_PATH**

   ```c
   / {
       soc {
           serial1: serial@40001000 {
               status = "okay";
               current-speed = <115200>;
               ...
       	};
       };
   };
   ```

   从路径获取节点

   可以使用下面方式获取节点

   ```c
   DT_PATH(soc, serial_40001000)
   ```

2. **DT_PROP**

   获取节点中的某个数据，还是上例可以使用

   ```c
   DT_PROP(DT_PATH(soc, serial_40001000), current_speed)
   ```

   获取到数值 115200

3. **DT_NODELABEL**

   将节点标签中的非字母数字字符转换为下划线以形成有效的 C 标记，并将所有字母小写。请注意，节点标签与标签属性不同。

   上例变更为

   ```c
   serial1: serial@40001000 {
       label = "UART_0";
       status = "okay";
       current-speed = <115200>;
       ...
   };
   ```

   此时可以使用 **DT_NODELABEL(serial1)** 获取当前节点

   注意这里字符串 `UART_0` 不是节点标签；它是名为 label 的属性的值。

   使用的话可以这样

   ```c
   DT_PROP(DT_NODELABEL(serial1), current_speed) // 115200
   ```

   同样是获取 115200

4. **DT_ALIAS(alias)**

   上例添加如下代码

   ```c
   aliases {
   	my-serial = &serial1;
   };
   ```

   此时，可以使用 **DT_ALIAS(my_serial)** 节点

5. **DT_INST**

   这个比较复杂

   示例代码

   ```c
   serial1: serial@40001000 {
       compatible = "vnd,soc-serial";
       status = "disabled";
       current-speed = <9600>;
   	...
   };
   serial2: serial@40002000 {
       compatible = "vnd,soc-serial";
       status = "okay";
       current-speed = <57600>;
   	...
   };
   serial3: serial@40003000 {
       compatible = "vnd,soc-serial";
       current-speed = <115200>;
       ...
   };
   ```

   禁用的节点分配最大的标签

   ```c
   // 表示的是 serial1 或者 serial2 无法确定
   DT_INST(0, vnd_soc_serial)
   DT_INST(1, vnd_soc_serial)
   
   // 表示的一定是 serial1
   DT_INST(2, vnd_soc_serial)
   ```

6. **DT_PARENT | DT_GPARENT | DT_CHILD**

   获取父节点的节点标识符, 获取祖父节点，获取子节点

7. **DT_NODE_PATH**

   获取设备树节点的完整路径作为字符串文字

   比如设备树如下

   ```c
   / {
       soc {
       	node: my-node@12345678 { ... };
       };
   };
   ```

   则

   ```c
   DT_NODE_PATH(DT_NODELABEL(node)) // "/soc/my-node@12345678"
   DT_NODE_PATH(DT_PATH(soc)) // "/soc"
   DT_NODE_PATH(DT_ROOT) // "/"
   ```

8. **DT_NODE_FULL_NAME**

   如果采用这个，上面的示例会变成

   ```c
   DT_NODE_FULL_NAME(DT_NODELABEL(node)) // "my-node@12345678"
   ```

### **特殊节点**

- /chosen

- /aliases

- /pinctrl

- /zephyr, user 无需写 device 就可以使用，方便用户开发的节点

可以直接在根节点中，不属于标准硬件节点

### 设备

```c
DEVICE_DT_GET(node id); //通过 node id 获取 device
DEVICEDT_GET_ANY(compat); //通过 compatible 获取 device
DEVICEDT_GET_ONE(compat); //通过 compatible 获取 device
DEVICE_GET(name); //通过 name (label 属性) 获取 device
DEVICE_DT_GET_ORNULL(nodeid); //判断节点是否 okey 状态

device_get_binding(name); //通过 name (label 属性) 获取 device
device_is_ready(device); //判断 device 结构是否可以使用
```



### 命名

**原文**

The recommended format is `"vendor, device" `, like `"avago, apds99a"` , or a sequence of these, like `"ti,hdc", "ti,hdc101e" `. The `vendor` part is an abbreviated name of the vendor. The file dts/bindings/vendor-prefixes.txt contains a list of commonly accepted `vendor` names. The ` device` part is usually taken from the datasheet.

**翻译**

推荐的格式是 '供应商，设备'，例如“avago，apds9960”，或这些格式的序列，例如“ti，hdc”，“ti，hdc1010”。供应商部分是供应商的缩写名称。文件 dts/bindings/vendor-prefixes.txt 包含普遍接受的供应商名称列表。器件部分通常取自数据表。



**原文**


lt is also sometimes a value like `gpio-keys` , ` mmio-sram`, or ` fixed-clock` when the hardware's behavioris generic.

**翻译**

当硬件行为是通用时，有时它也是一个值，如 GPIO-Keys、MMIO-SRAM 或固定时钟。

### 注册

reg
Information used to address the device. The value is specific to the device (ie. is different dependingon the compatible property).
The `reg` property is a sequence of `(address, length) ` pairs. Each pair is called a "register block".
Values are conventionally written in hex.
Here are some common patterns!

- Devices accessed via memory-mapped l/0 registers (like ` i2c@4eea3eee` ): `address` is usually thebase address of the l/0 register space, and `length` is the number of bytes occupied by the registers.

- 12C devices (like `apds996e@39` and its siblings): `address` is a slave address on the l2C bus. There isno `length` value.

- SPI devices: `address` is a chip select line number; there is no `length`



i2c40003000 是芯片中的 iic 地址

下面的是 iic 总线上的设备地址

一般涉及芯片的操作都是在 soc 节点下的。

### 绑定

zephyr build system 会从以下位置寻找绑定文件

- zephyr/dts/bindings/

- ${board_dir}/dts/bindings/

- ${project_dir}/dts/bindings/

也可以在 cmake 文件中添加

```cmake
 list(APPEND DTS_ROOT /path/to/your/dts)
```

用来指定目录

也可以在编译时，增加选项

```shell
west build -b <board_name> -- -DTS_ROOT=<path/to/your/dts>
```

如果自定义设备类型，可以把 yaml 文件添加到以上位置

文件名推荐和 `compatible` 一致，但不是必须的

## **信号（原子操作）**

### 信号量 Semaphores

<https://docs.zephyrproject.org/latest/kernel/services/synchronization/semaphores.html>

使用信号量来控制多个线程对一组资源的访问。使用信号量来同步生产和消费线程或 ISR 之间的处理。

- 类型：`k_sem`

- 定义：`k_sem_init()` or `K_SEM_DEFINE(name, initial_count, count_limit)`

- 给出：`k_sem_give()`

- 获取：`k_sem_take()`

- 获取计数：`k_sem_count_get()`

- 重置：`k_sem_reset()`

**举例**

```c
struct k_sem my_sem;
k_sem_init(&my_sem, 0, 1);
k_sem_give(&my_sem);
if (k_sem_take(&my_sem, K_MSEC(50)) != 0) {
    printk("Input data not available!");
} else {
    /* fetch available data */
    ...
}
```

### 互斥量 Mutexes

<https://docs.zephyrproject.org/latest/kernel/services/synchronization/mutexes.html>

使用互斥体提供对资源（例如物理设备）的独占访问。

- 类型：`k_mutex`

- 定义：`k_mutex_init() or K_MUTEX_DEFINE(my_mutex)`;

- 锁定：`k_mutex_lock()`

- 解锁：`k_mutex_unlock()`

  **说明**：无法锁定时则表示锁正在被使用，使用完成后需要解锁

  ```C
  if (k_mutex_lock(&my_mutex, K_MSEC(100)) == 0) {
      /* mutex sucessfully locked */
      } else {
      printf("Cannot lock XYZ displayn");
  }
  ```

### 互斥量 Futex

地址同互斥量 Mutex

k_futex 是一个轻量级互斥原语，旨在最大限度地减少内核参与。无竞争操作仅依赖于对共享内存的原子访问。 k_futex 作为内核对象进行跟踪，并且可以驻留在用户内存中，以便任何访问都绕过内核对象权限管理机制。

### 条件变量

地址：<https://docs.zephyrproject.org/latest/kernel/services/synchronization/condvar.html>

使用带有互斥体的条件变量来表示从一个线程到另一线程的状态（条件）变化。条件变量不是条件本身，也不是事件。该条件包含在周围的编程逻辑中。

互斥体本身并不是设计用作通知/同步机制的。它们旨在仅提供对共享资源的互斥访问。

- 类型：`k_condvar`

- 定义：`k_condvar_init or K_CONDVAR_DEFINE`

- 等待：`k_condvar_wait`

- 发出：`k_condvar_signal`

- 过程

1. 释放最后获取的互斥体。

2. 将当前线程放入条件变量队列中。

   **举例**

   ```c
   K_MUTEX_DEFINE(mutex);
   K_CONDVAR_DEFINE(condvar)
   int main(void)
   {
       k_mutex_lock(&mutex, K_FOREVER);
       /* block this thread until another thread signals cond. While
           * blocked, the mutex is released, then re-acquired before this
       * thread is woken up and the call returns.
           */
       k_condvar_wait(&condvar, &mutex, K_FOREVER);
       ...
       k_mutex_unlock(&mutex);
   }
   ```


## 数据传输

### Queue（底层实现）

<https://docs.zephyrproject.org/latest/kernel/services/data_passing/queues.html>

Zephyr 中的队列是一个实现传统队列的内核对象，允许线程和 **ISR** 添加和删除任意大小的数据项。队列类似于 FIFO，并且充当 k_fifo 和 k_lifo 的底层实现。有关使用的更多信息，请参阅 k_fifo。

- 定义：`k_queue_init or K_QUEUE_DEFINE`&nbsp;

- 尾插入队：`k_queue_append`

- 头插入队：`k_queue_prepend`

- 中间插队：`k_queue_insert`

- 出队：`k_queue_get`

- 判空：`k_queue_is_empty`

- 取消等待：`k_queue_cancel_wait`

### FIFOs（底层实现）

<https://docs.zephyrproject.org/latest/kernel/services/data_passing/fifos.html>

FIFO 是一个内核对象，它实现 **传统的先进先出** (FIFO) 队列，允许线程和 **ISR** 添加和删除任意大小的数据项。

定义：`k_fifo_init or K_FIFO_DEFINE`

入队：`k_fifo_put`

出队：`k_fifo_get`（带超时）

判空：`k_fifo_is_empty`

### LIFO（底层实现）

LIFO 是一个内核对象，它实现传统的 **后进先出** (LIFO) 队列，允许线程和 **ISR** 添加和删除任意大小的数据项。

- 定义：`k_lifo_init` or `K_LIFO_DEFINE`

- 入栈：`k_lifo_put`

- 出栈：`k_lifo_get`（带超时）

- 判空：`k_lifo_is_empty`

### 栈 Stacks

地址：<https://docs.zephyrproject.org/latest/kernel/services/data_passing/stacks.html>

**当存储项的最大数量已知时，使用堆栈以“后进先出”的方式存储和检索整数数据值。**

堆栈是一个内核对象，它实现传统的后进先出 (LIFO) 队列，允许线程和 ISR 添加和删除有限数量的整数数据值。

- 类型：`k_stack`

- 定义：`k_stack_init`

- 入栈：`k_stack_push`

- 出栈：`k_stack_pop`

- 

### 消息队列 MessageQueues

地址：[https://docs.zephyrproject.org/latest/kernel/services/data_passing/message_queues.html](https://docs.zephyrproject.org/latest/kernel/services/data_passing/message_queues.html#concepts)

**使用消息队列以异步方式在线程之间传输小数据项。**

是一个环形缓冲区，消息队列的 **环形缓冲区必须与 N 字节边界对齐**，其中 N 是 2 的幂（即 1、2、4、8、...）。为了确保存储在环形缓冲区中的消息类似地与此边界对齐，数据项大小也必须是 N 的倍数。数据项可以通过线程或 **ISR** 发送到消息队列。

- 类型：`k_msgq`

- 定义：`k_msgq_init` or `K_MSGQ_DEFINE`

- 入队：`k_msgq_put`

- 出队：`k_msgq_get`

- 查看队头：`k_msgq_peek`

**举例：**

```c
struct data_item_type {
    uint32_t field1;
    uint32_t field2;
    uint32_t field3;
};

char __aligned(4) my_msgq_buffer[10 * sizeof(struct data_item_type)];
struct k_msgq my_msgq;
k_msgq_init(&my_msgq, my_msgq_buffer, sizeof(struct data_item_type), 10);
```

或者

```c
K_MSGQ_DEFINE(my_msgq, sizeof(struct data_item_type), 10, 4);
```



### 邮箱 Mailboxes

地址：<https://docs.zephyrproject.org/latest/kernel/services/data_passing/mailboxes.html>

**当消息队列的能力不足时，使用邮箱在线程之间传输数据项。**

邮箱是一个内核对象，它提供了超出消息队列对象功能的增强消息队列功能。邮箱允许线程同步或异步发送和接收任何大小的消息。

- 类型：`k_mbox`

- 定义：`k_mbox_init` or `K_MBOX_DEFINE`

### 管道

地址：<https://docs.zephyrproject.org/latest/kernel/services/data_passing/pipes.html>

管道是一个内核对象，允许线程将字节流发送到另一个线程。管道可用于同步传输全部或部分数据块。

管道可以配置一个 **环形缓冲区**，用于保存已发送但尚未接收的数据；或者，管道可以没有环形缓冲区。

**使用管道在线程之间发送数据流。**
