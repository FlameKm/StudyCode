# QT

## Linux 安装

```shell
sudo apt install qtcreator
sudo apt install qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools
sudo apt-get install libqt5serialport5-dev libudev-dev
```

## 槽函数

参考资料：

[*QT 信号槽机制 - melonstreet - 博客园*](https://www.cnblogs.com/QG-whz/p/4995938.html)

### **定义**

```c
public slots:
    槽函数(); //接收方

signals:
    信号函数(); //发送方
```

### 举例：

```c
/**
* gameData：发送槽的类
* GameData:: dataUpdateComplete：发送的信号量，是一个空函数
* this：接受槽的类
* &MainWindow:: updateBoard：接受的槽函数
*/
connect(&gameData,&GameData::dataUpdateComplete,this,&MainWindow::updateBoard);
```

### **个人理解**：

槽类似回调函数，即前者的类型调用 **信号函数** 后会 **回调** 到接受类的 **槽函数**。

## 字符串

参考资料：

[*qt 中 double 转 QString 保留 n 位小数_qt double 保留小数位_GreenHandBruce 的博客-CSDN 博客*](https://blog.csdn.net/weixin_43935474/article/details/110536162)

[*Qt 的 QString 数据类型转换(整理)_qstring format_焕小谢的博客-CSDN 博客*](https://blog.csdn.net/qq_38832450/article/details/103162756)

### 数据类型转换为字符串

```c
//整数类型转字符串，浮点类型全转换
QString str = QString::number(data);

//浮点类型保留小数位数转换
QString str = QString::number(fData,'f',3);

//类似 printf 的方法
QString str = QString("hello %1 %2").arg(1).arg(2.2); //str = “hello 1 2.2”
QString str = QString("hello %1").arg(QString::number(fData,'f',3));

//多个数据
str = QString("cLapTime %1:%2:%3").arg(gameData.cLapTime[0]).arg(gameData.cLapTime[1]).arg(gameData.cLapTime[2]);
```



### 字符串转数据类型

```c
bool ret;
QString str = "15.54";
double val = str.toDouble(); //举例一 val = 15.54
float val = str.toDouble(&ok); //举例二 val = 15.54f ok = true
QString str = "FF";
float val = str.toInt(&ok,16); //举例三 val = 255 ok = true 其中 16 为原类型进制
```

**前面补全零**

**.arg(hid-> key, 8, 16, QLatin1Char('0'))**

## 打包可执行文件（WIN）

打开 qt 对应编译器的命令行

比如打开 ![](/home/hyc/Project/StudyCode/Notes/media/image34.png)

输入

```shell
windeployqt dir/name.exe
```

## 模块

### 安装

```shell
sudo apt-get install libqt5serialport5-dev libudev-dev #串口模块
```

### 添加附加模组（串口为例）

在 find_package 中添加需要拓展的模块, 添加内容与 qmake 中的 QT + name 一致

```cmake
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets SerialPort) # SerialPort就是添加的库
```

链接文件

```cmake
target_link_libraries(
    dash.boardcmake PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt$QT_VERSION_MAJOR}::Core
    Qt$IQT_VERSION_MAJOR}::GUi
    Qt${QT_VERSION_MAJOR}::Widgets
    $JOT VERSTON MAJOR上::SerialPort # 添加的
    )
```

## Painter 画家

一般初始流程是：确定界面大小、确定零点、**设置画笔（颜色和类型）、求出图形、绘画。** 其中界面大小和零点不是每个子图形都需要绘制的。

### 注意事项

使用完需要释放内存，不能够在已存在的情况下再次新建一个类。即想象为单例模式，否则画家会出问题。

### 刷新

`update()`

QWidget 成员函数，每次调用的时候，会执行可继承函数 `void paintEvent(QPaintEvent *event);`

包括每次新建类的时候，会调用此函数刷新一次画面。

### 美术类型

**画刷和画笔**

`brush` 和 `pen`。区别在于一个是线一个是面。使用其中一个笔会覆盖另一个笔。

**颜色**

颜色有纯色 `Color(r,g,b,a)` 最后一个参数 a 是透明度，四个参数取值范围都是 0-255。

除了纯色外还有渐变色，都是通过 `setColorAt(pos，color)` 设置渐变色范围的。第一个参数是 0-1 的数字，0 表示设置类的 startAngel，1 表示结束的位置（最）。在构建的时候有一个参数类型就是 startAngel。

```c++
QConicalGradient(qreal cx, qreal cy, qreal startAngle); //锥形渐变色
QRadialGradient(qreal cx, qreal cy, qreal radius, qreal fx, qreal fy); //横向渐变色
```

举例：

```c++
//渐变色
QPainterPath gradientRing;
gradientRing.addEllipse(-_radius + 4, -_radius + 4, 2 * (_radius - 4), h: 2 *)
QRadialGradient radialGradient(0, 0, _radius + 4, 0, 0);
radialGradient.setColorAt(1,  QColor( 6, 148, 208));
radialGradient.setColorAt(0.75, Qt::transparent);
paintersetBrush(radialGradient):
painter-drawPath( path: outRing);
painter-restore():
```

**图形形状**

`QPainterPath` 类中的成员函数，

```c++
addEllipse(qreal x, qreal y, qreal w, qreal h); //椭圆

arcTo(qreal x, qreal y, qreal w, qreal h, qreal startAngle, qreal arcLength);//弧形扇面
```

### 设置零点

原本的零点是 0,0 可以通过下面函数移动, 移动后能够较为方便的通过半径绘画

```c
painter->translate(width / 2.0, height / 2.0); //移动
```

## **QWidget 控件类**

基本全部控件都是继承自此类，有很多通用的特性

### 设置位置和大小

**坐标**：`setGeometry(ax,ay,aw,ah)`

参数分别左上角位置 xy 轴, 宽度和高度。

**控件位置**：`setParent`，`QWidget(this)`

两个方法，一个是设置变量，一个是构建的时候设置。

传入类型为指针。

### 显示控件

`show()` 

### 自定义美工

见 **Painter 画家** 项

## 键盘鼠标

参考链接：[*https://wizardforcel.gitbooks.io/qt-beginning/content/12.html*](https://wizardforcel.gitbooks.io/qt-beginning/content/12.html)

在 `Qwidget` 类中，已经存在了两个回调虚函数，就是 `mousePressEvent` 和 `keyPressEvent`

只需要在控件中实现两个函数就能够使用了，以下是举例

```c++
protected:
	void paintEvent(QPaintEvent *event) override;
	void keyPressEvent(QKeyEvent *e) override;
	void mousePressEvent(QMouseEvent *e) override;
```

```c++
void MainBoard::mousePressEvent(MouseEvent *e)
{
	QWidget::mousePressEvent(e)
    emit pressExit();
}
void MainBoard::keyPressEvent(QKeyEvent *e)
{
QWidget::keyPressEvent( event: e) 
    switch (e-key()) {
        case Qt::Key_Escape:
        case Qt::Key_Space:
       		emit pressExit();
        	break;
        default:
        	break;
    }
}
                                 }
```

## 使用自定义字体

现有两款字体

`DS-Digital.TTF` and `HanYiZongYiTiJian.TTF`

### 添加进项目

新建 `qrc` 后缀文件，比如新建 `res.qrc`

内容如下

```html
<RCC>
    <qresource prefix="/">
        <file>res/font/HanYiZongYiTiJian.TTF</file>
        <file>res/font/DS-Digital.TTF</file>
    </qresource>
</RCC>
```

解释:

`/ ` 是前缀

`res/font/HanYiZongYiTiJian.TTF` 是路径

将项目添加进工程 (添加进 CMAKE)

### 加载字体

```c++
QFontDatabase::addApplicationFont(":res/font/DS-Digital.TTF");
QFontDatabase::addApplicationFont(":res/font/HanYiZongYiTiJian.TTF");
```

### 使用

```c++
QFont font("HanYiZongYiTiJian",18);
ui->textBrowser->setFont(font);
```

 **注意：设置时候的字体名不是文件名，需要打开 ttf 文件查看**

## **设置屏幕大小**

```C++
showFullScreen(); //全屏
setFixedSize(x,y); //设置固定大小
setGeometry(ax, ay, aw, ah); //设置初始位置和大小
setParent(); //设置附加在哪
```

## **定时器**

参考链接：<https://xie.infoq.cn/article/4a3c8068f8fde1105a2b72b51>

**使用 `QTimer` 类**

1.用 new 的方式创建一个 QTimer 对象。

```c++
QTimer *timer = new QTimer(this);
```

2.将定时器的溢出信号连接到自定义的槽函数。

```C++
connect(timer, &QTimer::timeout, this, &Myself::update);
```

3.启动定时器。

```C++
timer->start(1000);
```

函数原型为：`void start(int msec)` 参数为定时器时间间隔，单位毫秒。

也可以调用 ` timer->setInterval(1000);` 设置定时器时间间隔，然后调用 timer-> start(); 开启定时器。

4.停止定时器。

```c++
timer->stop();
```

**使用 QObject 类**

在需要开启定时器的地方直接调用 ` startTimer();`

该函数的声明为：

```c++
int startTimer(int interval, Qt::TimerType timerType = Qt::CoarseTimer);
```

该函数开启一个定时器，返回值是定时器的编号。

参数一为时间间隔，单位毫秒；

参数二为定时器的精确度：

`Qt::PreciseTimer`（精确的定时器，尽量保持毫秒精度，试图保持精确度在 1 毫秒）；

`Qt::CoarseTimer`（粗略的定时器，尽量保持精度在所需的时间间隔 5%范围内）；

`Qt::VeryCoarseTimer`（很粗略的定时器，只保留完整的第二精度，大约为 500 毫秒）；

2.重载 

```c++
void QObject::timerEvent ( QTimerEvent * event );
```

当定时器溢出时，会自动响应 ` timerEvent()` 函数。

在 ` timerEvent()` 函数中，通过 `event->timerId()` 来确定是哪个定时器触发的；

3.在需要关闭定时器的地方调用 `killTimer();`

该函数的声明为： 

```c++
void killTimer(int Id);
```

该函数关闭一个定时器，参数为定时器的编号。

## **CLION 配置工具**

![](/home/hyc/Project/StudyCode/Notes/media/image43.png)

![](/home/hyc/Project/StudyCode/Notes/media/image44.png)

# 
