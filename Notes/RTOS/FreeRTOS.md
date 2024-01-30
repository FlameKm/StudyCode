# FREERTOS

Clion编译问题

```cmake
add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
```

这两行需要反注释,在CMAKE中

## **调度**

FreeRTOS 使用的是优先级抢占式调度算法。这意味着任务的优先级决定了它们被调度的顺序，优先级高的任务能够抢占正在执行的优先级低的任务。这种调度算法允许高优先级的任务在有需要的时候立即抢占 CPU 控制权，从而满足实时系统的需求。

高低优先级：抢占式

同等优先级：时间片

## 时间

### 绝对的延迟时间

源码中：

`TickType_t xLastWakeTime = xTaskGetTickCount()` //获取当前时间

`vTaskDelayUntil(&xLastWakeTime，3000)` //延时3秒

CUBEMX中 osTask

![](/home/hyc/Project/StudyCode/Notes/media/image24.png)

需要开启才可使用 本大点中的1，2点

### pdMS_TO_TICKS(ms)

这个宏的意义是将ms时间转换为ticks，当然在hal库生成的osdelay已经自动转换了，就可以不用使用这个宏。

### 获取当前时刻函数

`uint32_t xTaskGetTickCount()`

## 软件定时器

CUBEMX配置时候需要在Config parameters中开启Software timer definitions

![](/home/hyc/Project/StudyCode/Notes/media/image25.png)

freeRtos源码生成

```c
lockHandle = xTimerCreate("Lock Car",
                        2000,
                        pdFALSE,
                        (void *)0,
                        lockCarCallback);
                        checkHandle = xTimerCreate("Sensors Check",
                        100,
                        pdTRUE,
                        (void *)1,
                        checkCallback);
//必须要在 portMAX_DELAY 内开启 timer start
//portMAX_DELAY is listed as value for waiting indefinitely
//实际上0xFFFFFFFF 2^32-1 49天 7周
//在此期间，此task进入Block状态
xTimerStart(checkHandle, portMAX_DELAY);
```

## 多任务变量

1.  参数指针

CUBEMX生成时候选择指针名字(似乎不太方便)

即第三点的第四小点`pvParameters`

2.  锁住（相互排斥 Mutex）

![](/home/hyc/Project/StudyCode/Notes/media/image26.png)

不管是读操作还是写操作，它都各自是一个独立的task，这样用freertos运行多任务就会出现某个任务因为分配的时间到了，对数据的处理被迫中断，然后另一个任务又开始对数据进行操作，而这时的数据很可能只有一半是操作完成，另一半还未完成的状态，这样的数据状态会产生很大的运算错误，非常危险。所以一个参数只要有两个或以上task要对其进行操作，就必须上钥匙。

```c
SemaphoreHandle_t xHandler; //创建Handler
xHandler = xSemaphoreCreateMutex(); //创建一个MUTEX 返回NULL，或者handler
xSemaphoreGive(xHandler);// 释放
xSemaphoreTake(xHanlder, timeout); //指定时间内获取信号量 返回pdPASS, 或者pdFAIL
```

**一段不完整代码举例**

```c
SemaphoreHandle_t xMutexInventory = NULL;
void retailTask(void *pvParam) {
    while (1) {
        if (xSemaphoreTake(xMutexInventory, timeOut) == pdPASS) {
            //被MUTEX保护的内容叫做 Critical Section
            retailSale
            //释放钥匙
            xSemaphoreGive(xMutexInventory);
        }
    }
}
void onlineTask(void *pvParam) {
	while (1) {
    	if (xSemaphoreTake(xMutexInventory, timeOut) == pdPASS) {
            onlineSale(); //等待出售
            xSemaphoreGive(xMutexInventory);
    	}
	}
}
void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    xMutexInventory = xSemaphoreCreateMutex(); //创建MUTEX
    ...//创建任务
}
```

## 对CUBEMX创建的FreeRtos封装理解

```c
osThreadDef(task_main, Task_Main, osPriorityIdle, 0, 128);
task_mainHandle = osThreadCreate(osThread(task_main), NULL);
```

第一句的宏表示定义一个名为task_main的结构体，**osThread**(task_main)为获取结构体名字。通过结构体在osThreadCreate中使用常规的函数xTaskCreateStatic() 进行任务初始化。

## 任务管理

### 使用FreeRTOS源码创建任务

点击进入代码 [FREERTOS - TASK管理 - Wokwi Arduino and ESP32 Simulator](https://wokwi.com/projects/332867530330735188)

```c
/*任务创建函数*/
BaseType_t xTaskCreate(
    TaskFunction_tp vTaskCode, //函数指针
    const char * constpcName, //任务描述
    unsigned short usStackDepth, //堆栈大小
    void *pvParameters, //参数指针
    UBaseType_t uxPriority, //任务优先级
    TaskHandle_t *pvCreatedTask //回传句柄
);
```

1.  pvTaskCode： 函数指针，指向任务函数的入口。任务永远不会返回（位于死循环内）。该参数类型 TaskFunction_t 定义在文件 projdefs.h 中，定义为：typedef void(*TaskFunction_t)( void * ) ，即参数为空指针类型并返回空类型。

2.  pcName： 任务描述。主要用于调试。字符串的最大长度（包括字符串结束字符）由宏 configMAX_TASK_NAME_LEN 指定，该宏位于 FreeRTOSConfig.h 文件中。

3.  usStackDepth： 指定任务堆栈大小，能够支持的堆栈变量数量（堆栈深度），而不是字节数。比如，在 16 位宽度的堆栈下，usStackDepth 定义为 100，则实际使用 200 字节堆栈存储空间。堆栈的宽度乘以深度必须不超过 size_t 类型所能表示的最大值。比如，size_t 为16位，则可以表示堆栈的最大值是 65535 字节。这是因为堆栈在申请时是以字节为单位的，申请的字节数就是堆栈宽度乘以深度，如果这个乘积超出 size_t 所表示的范围，就会溢出。

4.  pvParameters： 指针，当任务创建时，作为一个参数传递给任务。

5.  uxPriority： 任务的优先级。具有 MPU 支持的系统，可以通过置位优先级参数的 portPRIVILEGE_BIT 位，随意的在特权（系统）模式下创建任务。比如，创建一个优先级为 2 的特权任务，参数 uxPriority 可以设置为 2 或者 portPRIVILEGE_BIT 。

6.  pvCreatedTask： 用于回传一个句柄（ID），创建任务后可以使用这个句柄引用任务。

举个例子，需要创建一个任务

```c
TaskHandle_t Task1_Handle;//1.定义一个句柄
void Task_1(void *arg);//2.对任务函数进行声明
xTaskCreate(Task_1, "Task1", 256, NULL, 6, &Task1_Handle);//3.创建任务
void Task_1(void *arg){
    for (;;) {
    	//3.函数内容
    }
}
```

例如

```c
if (xTaskCreate(radioBilibili, "Bilibili Channel", 1024 * 8, NULL, 1, &biliHandle) == pdPASS){}
```

### 任务删除

`void vTaskDelete( TaskHandle_t xTask );`

如何任务是用xTaskCreate()创建的，那么在此任务被删除以后此任务之前申请的堆栈和控制块和控制内存会在空闲任务中被释放掉，因此当调用函数vTaskDelete()删除任务以后必须给空闲任务一定的运行时间。

### 任务暂停

`void vTaskSuspend( TaskHandle_t xTaskToSuspend );`

### 任务恢复

`void vTaskResume( TaskHandle_t xTaskToResume );`

### 注意

以上所有任务共同配合时，可以使用`TaskHandle_t biliHandle = NULL`; //Task Handler一个handle

注意在删除任务前，一定要确保任务是存在的

删除不存在的任务，比如连续删除两次，自动重启

创建时需要判断是否之前已经创建了Bilibili channel task， 如果没有创建，则创建该Task

## 任务优先级

[**FREERTOS - 任务优先级 - Wokwi Arduino and ESP32 Simulator**](https://wokwi.com/projects/332897591021797971)

### 设置

`vTaskPrioritySet(xFirstClassHandle, 2);`

### 创建任务附带优先级

`xTaskCreatePinnedToCore(ecoClass, "ecoClass", 1024 * 2, NULL, 1, NULL, 1);`

### 资源退让

`taskYIELD();`//资源退让给同等级或者更高级的任务

### 查询当前任务优先级

`UBaseType_t uTaskPriority = uxTaskPriorityGet(NULL);`

## 信号量

### 二进制信号量

```c
SemaphoreHandle_t xSemaName = NULL; //创建信号量Handler
xSemaName = xSemaphoreCreateBinary(); //创建二进制信号量
if (xSemaphoreTake( xSemaLED, timeOut) == pdTRUE )//获得了信号量过来则进入
	xSemaphoreGive(xSemaLED); //正常给出信号
xSemaphoreGiveFromISR(xSemaLED, NULL); //外设中断给出信号
```

二进制信号量的好处是,等待的时候会执行其它程序,相比单纯的bool数据,它优点很多

### 计数信号量

```c
SemaphoreHandle_t xSemaName = NULL;
xSemaName = xSemaphoreCreateCounting(3, 0); //3表示上限,0表示初始值
if (xSemaphoreTake(xSemaPhone, portMAX_DELAY) == pdTRUE ) //portMAX_DELAY表示无限等待
xSemaphoreGive(xSemaPhone);
```

## 事件组

**能不用就不用**

### 等待

[FREERTOS - EVENTS WAIT BITS - Wokwi Arduino and ESP32 Simulator](https://wokwi.com/projects/333436492452987474)

```c
// 初始化
EventGroupHandle_t xEventPurchase = NULL; //创建event handler(全局变量)
xEventPurchase = xEventGroupCreate(); //创建 event group

// 等待函数
uxBits = xEventGroupWaitBits (xEventPurchase, //Event Group Handler
                            ADDTOCART_0 | PAYMENT_1 | INVENTORY_2,//等待Event Group中的那个Bit(s)
                            pdFALSE, //执行后，对应的Bits是否重置为 0
                            pdTRUE, //等待的Bits判断关系 True为 AND, False为 OR
                            xTimeOut);
uxBits = xEventGroupSetBits(xEventPurchase, PAYMENT_1); // 将bit1 PAYMENT_1 设置为
```



### 同步SYNC

[FREERTOS - EVENTS SYNC - Wokwi Arduino and ESP32 Simulator](https://wokwi.com/projects/333430026744627794)

```c
uxBits = xEventGroupSync (xEventPurchase, //Event Group Handler
                          0x01, // 先将这个bit(s)设置为 1,然后再等待
                          0x07, //等待这些bits为 1
                          xTimeOut);
if ((uxBits & BOUGHT_PAID_SENT) == BOUGHT_PAID_SENT) {
//next step;
}
```

这个是设置某个位后等待

## 任务通知

### 创建

```c
TaskHandle_t xflashLED = NULL;//全局变量
xTaskCreate(flashLED, "Flash LED", 1024 * 4, NULL, 1, &xflashLED);
```

### 取代二进制信号量

```c
//命令含义，相当于精简化的 xTaskNotify() + eIncrement
xTaskNotifyGive(xflashLED);
uint32_t ulNotificationValue; //用于获取数值

//命令含义: waitting for notification, then reset
ulNotificationValue = ulTaskNotifyTake(pdTRUE, //pdTRUE 运行完后，清零
portMAX_DELAY);
if ( ulNotificationValue > 0 ){
//Do something
}
```

### 取代信号组

```c
// 任务一中:
xTaskNotify(TaskHandle,.0b01,eSetBits);

// 任务二中:
xTaskNotiffy(TaskHandle,.0b10,eSetBits);

// 任务三中
xTaskNotifyWait(0x00000000,//进入前不清除
                0xFFFFFFFF, //进入后全部清除
                &num, //传入到NUM
                portMAX_DELAY
);
//or
xTaskNotifyWait(0x0, 0x0, &num, portMAX_DELAY);
    if (num == 0b11){
}
```

### 模拟队列

只能模拟长度为一的队列

## 消息队列

动态创建

```c
QueueHandle_t xQueueCreate(UBaseType_t uxQueueLength, UBaseType_t uxItemSize);
//传入参数（队列长度，每个数据的大小：以字节为单位）
//返回 非零：创建成功返回消息队列的句柄 NULL：创建失败
```

静态创建

```c
QueueHandle_t xQueueCreateStatic(UBaseType_t uxQueueLength,
                                 UBaseType_t uxItemSize,
                                 uint8_t *pucQueueStorageBuffer,
                                 StaticQueue_t *pxQueueBuffer);
```

队列清空

```c
BaseType_t xQueueReset( QueueHandle_t pxQueue);//传入队列句柄即可
```

删除队列

```c
void vQueueDelete( QueueHandle_t xQueue );//传入队列句柄即可
```

写队列

```c
BaseType_t xQueueSend(QueueHandle_t xQueue,const void *pvItemToQueue,TickType_t xTicksToWait);

BaseType_t xQueueSendToBack(QueueHandle_t xQueue,const void *pvItemToQueue,TickType_t xTicksToWait);

// 中断写入(保护线程)
BaseType_t xQueueSendToBackFromISR(QueueHandle_t xQueue,const void *pvItemToQueue,BaseType_t, *pxHigherPriorityTaskWoken);

BaseType_t xQueueSendToFrontFromISR(QueueHandle_t xQueue,const void *pvItemToQueue,BaseType_t, *pxHigherPriorityTaskWoken);
```

读队列

```c
BaseType_t xQueueReceive( QueueHandle_t xQueue,void * const pvBuffer,TickType_t xTicksToWait );

// 中断读
BaseType_t xQueueReceiveFromISR(QueueHandle_t xQueue, void *pvBuffer,BaseType_t *pxTaskWoken);

// 偷窥--获得当前数据
BaseType_t xQueuePeek(QueueHandle_t xQueue,void * const pvBuffer,TickType_t xTicksToWait);

//中断
BaseType_t xQueuePeekFromISR(QueueHandle_t xQueue, void *pvBuffer)
```

查询

```c
UBaseType_t uxQueueMessagesWaiting( const QueueHandle_t xQueue );//返回队列中可用数据的个数

UBaseType_t uxQueueSpacesAvailable( const QueueHandle_t xQueue );//返回队列中可用空间的个数
```

