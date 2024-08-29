# 单片机（STM32 为主）

## 编码器

### 相关函数

```c
void TIM_EncoderInterfaceConfig(TIM_TypeDef* TIMx, uint16_t TIM_EncoderMode, uint16_t TIM_IC1Polarity, uint16_t TIM_IC2Polarity)
```

`TIMx` 参数就是使用哪个定时器作为编码器接口的捕捉定时器.

`TIM_EncoderMode` 参数是模式, 是单相计数（只能反映速度）还是两相计数（速度和方向）.

`TIM_IC1Polarity` 和 `TIM_IC2Polarity` 参数就是通道 1、2 的捕捉极性.

![](/home/hyc/Project/StudyCode/Notes/media/image1.png)

图 1-1

## PWM

### 注意事项

![](/home/hyc/Project/StudyCode/Notes/media/image2.png)

### 代码框架

```c
//TIM1 CH1 PWM 输出设置
//PWM 输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM1_PWM_Init(u16 arr,u16 psc)
{
GPIO_InitTypeDef GPIO_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //使能 TIMx 外设
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能 PA 时钟
//设置该引脚为复用输出功能, 输出 TIM1 CH1 的 PWM 脉冲波形
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM1_CH1
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用功能输出
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化 GPIO
TIM_TimeBaseStructure.TIM_Period = arr; //设置自动重装载周期值
TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置预分频值 不分频
TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割: TDTS = Tck_tim
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //初始化 TIMx
TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //CH1 PWM2 模式
TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //OC1 低电平有效
TIM_OC1Init(TIM1, &TIM_OCInitStructure); //根据指定的参数初始化外设 TIMx
TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable); //CH1 预装载使能
TIM_ARRPreloadConfig(TIM1, ENABLE); //使能 TIMx 在 ARR 上的预装载寄存器
TIM_CtrlPWMOutputs(TIM1,ENABLE); //MOE 主输出使能, 高级定时器必须开启
TIM_Cmd(TIM1, ENABLE); //使能 TIMx
}
```

## **复用功能**

中文手册 105 页

## 时钟

在 STM32 中

APB1(低速外设)上的设备有：电源接口、备份接口、CAN、USB、I2C1、I2C2、UART2、UART3、SPI2、窗口看门狗、Timer2、Timer3、Timer4 。

APB2(高速外设)上的设备有：GPIO_A-E、USART1、ADC1、ADC2、ADC3、TIM1、TIM8、SPI1、ALL。

具体参考时钟树

## CAN

参考链接

https://zhuanlan.zhihu.com/p/548772223

CAN 不能与 USB 从设备同时使用，因为他们共用一组 RAM，如果需要使用 USB 功能时，需要调用关闭 CAN 总线的初始化函数。需要使用 CAN 的时候，需要调用开启 CAN 总线的初始化函数。

在 HAL 库中，

```c
HAL_CAN_MspDeInit(&hcan); //关闭
HAL_CAN_MspInit(&hcan)； //初始化
```

### 标准帧

### 报文

报文是短帧结构，短的传送时间使其受干扰概率低，CAN 有很好的校验机制，这些都保证了 CAN 通信的可靠性。

### 邮箱

有两个队列，fifo0 和 fifo1，每一个队列又存在三个邮箱 BOX1-BOX3。

fifo0 和 fifo1 不同的中断

### 波特率

![](/home/hyc/Project/StudyCode/Notes/media/image3.png)

波特率 = PCLK1 时钟 / （（1 + 7 + 8）*9）

### 过滤器

### 模式

Lookback，用于测试代码，无法从接收外部数据

### **过滤器**

```c
static void CANFilter_Config(void)
{
    CAN_FilterTypeDef sFilterConfig;
    sFilterConfig.FilterBank = 0; //CAN 过滤器编号，范围 0-27
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK; //CAN 过滤器模式，掩码模式或列表模式
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT; //CAN 过滤器尺度，16 位或 32 位
    sFilterConfig.FilterIdHigh = 0x000 << 5; //32 位下，存储要过滤 ID 的高 16 位
    sFilterConfig.FilterIdLow = 0x0000; //32 位下，存储要过滤 ID 的低 16 位
    sFilterConfig.FilterMaskIdHigh = 0x0000; //掩码模式下，存储的是掩码
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = 0; //报文通过过滤器的匹配后，存储到哪个 FIFO
    sFilterConfig.FilterActivation = ENABLE; //激活过滤器
    sFilterConfig.SlaveStartFilterBank = 0;
    if (HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK) {
        printf("CAN Filter Config Fail!rn");
        Error_Handler();
    }
    printf("CAN Filter Config Success!rn");
}
```

## **固件升级**

参考链接：

<https://zhuanlan.zhihu.com/p/73037424>

CMSIS-DAP 调试器是有单独的一条复位线的，但是当时没考虑它可能采用了这种方式，只考虑可能采用了内核复位

## ADC

### 周期

采集固定 12.5 个周期，间隔可以选择 ![](/home/hyc/Project/StudyCode/Notes/media/image4.png)，计算采样频率的时候为：ADC 时钟频率/（55.5+12.2）

## BootLoader

### 跳转方式

```c
typedef void (*fp_t)(void);

__IO uint32_t BootloaderAddr = 0x08004000; *// 32k = 0x8000*

void jump_app()

{

volatile uint32_t jump_addr;

fp_t jump;

jump_addr = *(volatile uint32_t *) (BootloaderAddr + 4);

jump = (fp_t) jump_addr;

__set_MSP(*(volatile uint32_t *) BootloaderAddr);

jump();

}
```

此时可以跳转，但是跳转后会出现问题，笔者的开发环境下中断会在 bootload 程序中，可以定位问题为中断定位错误，解决方式是将 APP 程序的中断向量表进行偏移，具体方式请查看 **中断向量表** 一节。

上面值得注意的有跳转+4 是表示前面有中断跳转，此时才是真正的 main 函数开端

### 跳转前准备

需要清除全部中断与中断标志位。

### 程序烧录 XModem、YModem

两种烧录协议

## 中断向量表

更改中断向量表起始地址，需要取消注释，并修改偏移地址，修改的文件为 system_stm32f1xx.c。

![](/home/hyc/Project/StudyCode/Notes/media/image5.png)

stm32cube+clion 环境中，修改.ld 文件可以修改烧录起始地址

![](/home/hyc/Project/StudyCode/Notes/media/image6.png)

# STM32Cube MX

## 初尝试

### 晶振配置

![](/home/hyc/Project/StudyCode/Notes/media/image7.png)

### 工程

![](/home/hyc/Project/StudyCode/Notes/media/image8.png)

## 串口

原文链接：<https://blog.csdn.net/as480133937/article/details/99073783>

### 配置

![](/home/hyc/Project/StudyCode/Notes/media/image9.png)

### 中断

![](/home/hyc/Project/StudyCode/Notes/media/image10.png)

### HAL 库 UART 函数库介绍

UART 结构体定义

`UART_HandleTypeDef huart1;`

UART 的名称定义，这个结构体中存放了 UART 所有用到的功能，后面的别名就是我们所用的 uart 串口的别名，默认为 huart1 ![IMG_256](/home/hyc/Project/StudyCode/Notes/media/image11.png)

### UART 接收中断

原文链接：

<https://blog.csdn.net/as480133937/article/details/99073783>

中断函数

`void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)`

### 串口发送/接收函数

`HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1)` 

`HAL_UART_Transmit()` //串口发送数据，使用超时管理机制

`HAL_UART_Receive()`//串口接收数据，使用超时管理机制

`HAL_UART_Transmit_IT()` //串口中断模式发送

`HAL_UART_Receive_IT()` //串口中断模式接收

`HAL_UART_Transmit_DMA()`//串口 DMA 模式发送`

`HAL_UART_Transmit_DMA()`//串口 DMA 模式接收

这几个函数的参数基本都是一样的，我们挑两个讲解一下

串口发送数据：

`HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout)`

功能：串口发送指定长度的数据。如果超时没发送完成，则不再发送，返回超时标志（HAL_TIMEOUT）。

参数：

`UART_HandleTypeDef *huart ` UATR 的别名

 如 : UART_HandleTypeDef huart1; 别名就是 `huart1`

`*pData` 需要发送的数据

`Size` 发送的字节数

`Timeout` 最大发送时间，发送数据超过该时间退出发送

举例：` HAL_UART_Transmit(&huart1, (uint8_t *)ZZX, 3, 0xffff);`//串口发送三个字节数据，最大传输时间 0xffff

中断接收数据：

`HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)`

功能：串口中断接收，以中断方式接收指定长度数据。

大致过程是，设置数据存放位置，接收数据长度，然后使能串口接收中断。接收到数据时，会触发串口中断。

再然后，串口中断函数处理，直到接收到指定长度数据，而后关闭中断，进入中断接收回调函数，不再触发接收中断。(只触发一次中断)

### 重新定义 printf 函数

在 stm32f4xx_hal.c 中包含#include <stdio.h>

在 stm32f4xx_hal.c 中重写 fget 和 fput 函数

```c
#include "stm32f4xx_hal.h"
#include <stdio.h>
extern UART_HandleTypeDef huart1; //声明串口

/**
* 函数功能: 重定向 c 库函数 printf 到 DEBUG_USARTx
* 输入参数: 无
* 返 回 值: 无
* 说 明：无
*/
int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
    return ch;
}
/**
* 函数功能: 重定向 c 库函数 getchar, scanf 到 DEBUG_USARTx
* 输入参数: 无
* 返 回 值: 无
* 说 明：无
*/
int fgetc(FILE *f)
{
    uint8_t ch = 0;
    HAL_UART_Receive(&huart1, &ch, 1, 0xffff);
    return ch;
}
```

### 串口再使用的笔记

配置好需主动开启中断

![](/home/hyc/Project/StudyCode/Notes/media/image12.png)

通过自己写的函数实现获取数据

注意最后一句

![](/home/hyc/Project/StudyCode/Notes/media/image13.png)

文件在 stm32f1xx_it.c 和 stm32f1xx_it.h 里面

![](/home/hyc/Project/StudyCode/Notes/media/image14.png)

注意再调用

###  DMA 串口使用

**配置**

![](/home/hyc/Project/StudyCode/Notes/media/image15.png)

Normal 正常模式，DMA 发送一次就停止发送；

Circular 循环模式，会一直发送数据；

**发送函数:**

``HAL_UART_Transmit_DMA( &huart6, (uint8_t *)"hello DISCO by DMArn", sizeof("hello DISCO by DMArn") );``

**注意事项:**

打开 DMA 会自动开启 DMA 中断, 此时应该打开对应串口中断。

## PWM

![](/home/hyc/Project/StudyCode/Notes/media/image16.png)

![](/home/hyc/Project/StudyCode/Notes/media/image17.png)

`HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);`

![](/home/hyc/Project/StudyCode/Notes/media/image18.png)

or

`__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, pwmVal);`

## ADC

### 单通道采样（不用 DMA）

![](/home/hyc/Project/StudyCode/Notes/media/image19.png)

使用函数

Init：

`HAL_ADCEx_Calibration_Start(&hadc1);`

Begin：

```c
HAL_ADC_Start(&hadc1);//启动 ADC 转换
HAL_ADC_PollForConversion(&hadc1, 50);//等待转换完成，50 为最大等待时间，单位为 ms
```

Get：

```c
if (HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC)) {
    adcValue = HAL_ADC_GetValue(&hadc1); //获取 AD 值
    logi("ADC1 Reading : %d", adcValue);
}
```

### 多通道（非 DMA）

![](/home/hyc/Project/StudyCode/Notes/media/image20.png)

如图设置，只需要按照 1 的方法多次读取即可。

轮询读取

### 多通道（DMA）

#### 配置

需要先 **开启连续转换模式**

![](/home/hyc/Project/StudyCode/Notes/media/image21.png)

1.  高级优先级

2.  循环模式，读取完成后继续重新读取。

3.  勾选寄存器模式：如果不勾选，每次都只存放在一个寄存器。勾选后，每次存放的地址增加

4.  16 位宽带寄存器，12 位的 ADC 足够。

#### 使用

Init：

```C
HAL_ADCEx_Calibration_Start(&hadc1); //校准
uint16_t adcValue[2];
HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adcValue, 2);
```

Get：

直接使用数据。

#### 注意

1.  自动生成的 DMA 和 ADC 的初始化顺序不能相反。先 DMA 再 ADC

![](/home/hyc/Project/StudyCode/Notes/media/image22.png)

2.  小心一直进入中断！

3.  传输的数值选择的是半字节的话，需要对应的内存去存储

## IIC

默认配置即可

发送函数

方法一

```c
while (HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x00,I2C_MEMADD_SIZE_8BIT, &IIC_Command,sizeof(IIC_Command), 10) != HAL_OK) {
    if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF) {
    Error_Handler();
    }
}
```

方法二

发送单个数据, 数据的第一个数据为内存地址

0x78 是设备地址

`HAL_I2C_Master_Transmit(&hi2c1,0x78, c, *sizeof*(c), 10)`

方法三

需要在 CUBEMX 配置 slave 的地址

```c
uint8_t c[2] = {0x40, IIC_Data};
HAL_I2C_Slave_Transmit(&hi2c1, c, *sizeof*(c), 10);
```

## 定时器

定时器回调函数：

`void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)`

中断

`HAL_TIM_Base_Start_IT`

PWM

`HAL_TIM_PWM_Start`

## CAN

### 时钟

使用 APB1 时钟

![](/home/hyc/Project/StudyCode/Notes/media/image23.png)

### 发送

添加到邮箱

`HAL_StatusTypeDef HAL_CAN_AddTxMessage(CAN_HandleTypeDef *hcan, const CAN_TxHeaderTypeDef *pHeader, const uint8_t aData[], uint32_t *pTxMailbox)`

### 中断



# **计算机语言**

## **C 语言**

### 优先级

| 优先级 | 运算符                                                       | 结合律   |
| ------ | ------------------------------------------------------------ | -------- |
| 1      | 后缀运算符：[]   ()   ·   ->   ++   --(类型名称){列表}       | 从左到右 |
| 2      | 一元运算符：++   --   !   ~   +   -   *   &   sizeof_Alignof | 从右到左 |
| 3      | 类型转换运算符：(类型名称)                                   | 从右到左 |
| 4      | 乘除法运算符：*   /   %                                      | 从左到右 |
| 5      | 加减法运算符：+   -                                          | 从左到右 |
| 6      | 移位运算符：<<   > >                                         | 从左到右 |
| 7      | 关系运算符： <  <=   >  >=                                   | 从左到右 |
| 8      | 相等运算符：==   !=                                          | 从左到右 |
| 9      | 位运算符 AND：&                                              | 从左到右 |
| 10     | 位运算符 XOR：^                                              | 从左到右 |
| 11     | 位运算符 OR：\|                                              | 从左到右 |
| 12     | 逻辑运算符 AND：&&                                           | 从左到右 |
| 13     | 逻辑运算符 OR：\|\|                                          | 从左到右 |
| 14     | 条件运算符：?:                                               | 从右到左 |
| 15     | 赋值运算符：   =     +=     -=    *=    /=    %=    &=    ^=    \|=   <<=    > >= | 从右到左 |
| 16     | 逗号运算符：，                                               | 从左到右 |

### qsort

函数用法

```c
qsort(buf, n, sizeof(int16_t), filter_cmp);
```

`filter_cmp` 是比较函数

具体为

```c
int openmv_filter_cmp(const void *a, const void *b) {
	return (*(int16_t *) a - *(int16_t *) b);
}
```

这个表示：

a > b 的话则替换，即从小到大排序

### 数据类型

wchar_t 直译过来是宽字符，在 linux 系统中 4 字节，在 windows 中 2 字节

参考链接：[*https://zh.wikipedia.org/wiki/%E5%AF%AC%E5%AD%97%E5%85%83*](https://zh.wikipedia.org/wiki/%E5%AF%AC%E5%AD%97%E5%85%83)

### 编译器优化

| 优化等级 | 优化内容                                                     |
| :------: | ------------------------------------------------------------ |
|   -O1    | 在不影响编译速度的前提下，尽量采用一些优化算法降低代码大小和可执行代码的运行速度。 |
|   -O2    | 该优化选项会牺牲部分编译速度，除了执行-O1 所执行的所有优化之外，还会采用几乎所有的目标配置支持的优化算法，用以提高目标代码的运行速度。 |
|   -O3    | 该选项除了执行-O2 所有的优化选项之外，一般都是采取很多向量化算法，提高代码的并行执行程度，利用现代 CPU 中的流水线，Cache 等。这个选项会提高执行代码的大小，当然会降低目标代码的执行时间。 |
|   -Os    | 这个选项是在-O2 的基础之上，尽量的降低目标代码的大小，这对于存储容量很小的设备来说非常重要。 |
|  -Ofast  | 该选项将不会严格遵循语言标准，除了启用所有的-O3 优化选项之外，也会针对某些语言启用部分优化。 |
|   -Og    | 该标识会精心挑选部分与-g 选项不冲突的优化选项，当然就能提供合理的优化水平，同时产生较好的可调试信息和对语言标准的遵循程度。 |

## **JAVA**

### 安装

```shell
sudo apt install openjdk-xx-jdk
```

### 选择版本

```shell
sudo update-alternatives --config java
```

## OOP FOR C++

### 多态

参考链接：[override finial - 多态 - Wokwi Arduino and ESP32 Simulator](https://wokwi.com/projects/336414219117265492)

精简做法：

父类: `virtual` 放在 函数 + `destructor`

子类: `override` 放在 函数

推荐：

父类: `virtual` 放在 函数 + `destructor`

子类: ` virtual` 放在 函数 + `destructor`， `override` 放在 函数

# **数据结构和算法**

## 抽象数据类型 ATD

参考链接：

<https://book.itheima.net/course/223/1276707762369208322/1276707936655122434>

抽象数据类型有两个重要特征：数据抽象和数据封装。

能够很大程度上实现代码的 **移植性、通用性和拓展性**。

所谓数据抽象是指用 ADT 描述程序处理的实体时，强调的是其本质的特征，无论内部结构如何变化，只要本质特性不变，就不影响其外部使用。例如，在程序设计语言中经常使用的数据类型 int，它就可以理解为是一个抽象数据类型，在不同的计算机或操作系统中，它的实现方式可能会有所不同，但它本质上的数学特性是保持不变的， int 类型的数据指的是整数，可以进行加减乘除模等一些运算，int 类型数据的这些数学特性保持不变，那么在编程者来看，它们都是相同的。因此数据抽象的意义在于数据类型的数学抽象特性。

而另一方面，所谓的数据封装是指用户在软件设计时从问题的数学模型中抽象出来的逻辑数据结构和逻辑数据结构上的运算，需要通过固有数据类型（高级编程语言中已实现的数据类型）来实现，它在定义时必须要给出名字及其能够进行的运算操作。一旦定义了一个抽象数据类型，程序设计中就可以像使用基本数据类型那样来使用它。例如，在统计学生信息时，经常会使用姓名、学号、成绩等信息，我们可以定义一个抽象数据类型“student”，它封装了姓名、学号、成绩三个不同类型的变量，这样我们操作 student 的变量就能很方便地知道这些信息了。C 语言中的结构体以及 C++语言中的类等都是这种形式。

## **PID 算法**

### P（比例）

比例控制

### I（积分）

误差累计

### D（微分）

PD 控制: ``U(t) = Kp * err(t) + Kd * derr(t)/dt``

### 增量式和位置式

参考链接：[(70 条消息) 位置式 PID 与增量式 PID 区别浅析_Z 小旋的博客-CSDN 博客_增量式 pid 和位置式 pid 的区别](https://blog.csdn.net/as480133937/article/details/89508034)

增量式

![IMG_256](/home/hyc/Project/StudyCode/Notes/media/image27.png)

位置式

![IMG_256](/home/hyc/Project/StudyCode/Notes/media/image28.png)

**位置式 PID 优缺点：**

**优点：**

位置式 PID 是一种非递推式算法，可直接控制执行机构（如平衡小车），u(k)的值和执行机构的实际位置（如小车当前角度）是一一对应的，因此在执行机构不带积分部件的对象中可以很好应用

**缺点：**

每次输出均与过去的状态有关，计算时要对 e(k)进行累加，运算工作量大。

**增量式 PID 优缺点**：

优点：

1. 误动作时影响小，必要时可用逻辑判断的方法去掉出错数据。

2. 手动/自动切换时冲击小，便于实现无扰动切换。当计算机故障时，仍能保持原值。

3. 算式中不需要累加。控制增量Δ u(k)的确定仅与最近 3 次的采样值有关。

缺点：

1. 积分截断效应大，有稳态误差；

2. 溢出的影响大。有的被控对象用增量式则不太好；

### 调参口诀

**实际应用，进行 PID 参数调节时，一般使用试凑法，PID 参数整定口诀如下：**

参数整定找最佳，从小到大顺序查，

先是比例后积分，最后再把微分加，

曲线振荡很频繁，比例度盘要放大，

曲线漂浮绕大湾，比例度盘往小扳，

曲线偏离回复慢，积分时间往下降，

曲线波动周期长，积分时间再加长，

曲线振荡频率快，先把微分降下来，

动差大来波动慢，微分时间应加长，

理想曲线两个波，前高后低 4 比 1，

一看二调多分析，调节质量不会低。

## 环形缓冲区

### 裸机中利用 DMA 使用环形缓冲区实现异步发送



```c
/* 定义数据 */
static uint8_t uart_buffer[UART_BUFFER_SIZE];
static uint16_t left, right;
static uint16_t pack_size; //每次出队个数
/**
* 发送中断回调, 用于维护环形缓冲区
* @param huart
* @param size
*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == uart_handle) {
        left = (left + pack_size) % UART_BUFFER_SIZE;
        transmit_mutex = false;
        if (left != right) { //检查环形队列中是否还有数据
            uart_transmit_start();
        }
    }
}
/**
* 发送当前需要发送的数据
* @return
*/
static int uart_transmit_start()
{
    HAL_StatusTypeDef ret;
    if (transmit_mutex == true) {
    	return -1;
    }
    transmit_mutex = true;
    // 计算发送的个数
    if (right > left) {
    	pack_size = right - left;
    }
    else {
    	pack_size = UART_BUFFER_SIZE - left;
    }
    ret = HAL_UART_Transmit_DMA(uart_handle, (uint8_t *) uart_buffer + left, pack_size);
    if (ret != HAL_OK) {
    	return -1;
    }
    return 0;
}
/**
* 添加数据到环形缓冲区，这个函数是 api 的接口
* @param com
* @param buf
* @param size
* @return
*/
static int uart_transmit(com_buffer_cb_t *com, uint8_t *buf, uint16_t size)
{
    UNUSED(com);
    int err;
    int next_index = (right + size) % UART_BUFFER_SIZE;
    // *保存数据
    if (next_index > right) { // 没有超限循环
        if (next_index >= left && right < left) {
        	return -1;
        }
        memcpy(uart_buffer + right, buf, size);
    }
    else { // 超限循环
        if (next_index > left && right > left) {
       		return -1;
        }
        memcpy(uart_buffer + right, buf, UART_BUFFER_SIZE - right);
        memcpy(uart_buffer, buf + UART_BUFFER_SIZE - right, size - (UART_BUFFER_SIZE - right));
    }
    right = next_index;
    // *开启尝试发送
    err = uart_transmit_start();
    return err;
}
```

# 其它(单片机)

## 高级定时器注意事项

![](/home/hyc/Project/StudyCode/Notes/media/image29.png)

## ROM 烧录

早期一般是将调试好的单片机程序写入到 ROM、EPROM, 中，这种操作就像刻制光盘一样，实在高电压方式下写入，PROM%20 是一次性写入，存储内部发生变化，有些线路或元件就被烧断，不可再恢复，所以叫做烧写，EPROM%20 可以使用紫外线将原来写入的内容擦除，重新烧写，目前大量采用%20EEPROM，是可以电擦写的存储器。 

