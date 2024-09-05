## Linux Base

### 开发环境

参考链接：<https://zhuanlan.zhihu.com/p/100367053>

![](/home/hyc/Project/StudyCode/Notes/media/image30.png)

### 基础命令

指的是自己用的多的命令，会随学习程度增加

| 命令     | 内容                   |
| -------- | ---------------------- |
| cd       | 切换目录               |
| ls       | 输出目录               |
| ifconfig | 输出当前网络状态       |
| vim      | vim 编辑器             |
| touch    | 新建文件               |
| pwd      | 输出当前路径的绝对路径 |
| cp       | 复制                   |
| rm       | 删除                   |
| mv       | 移动                   |
| unzip    | 解压 zip 文件          |
| chmod    | 修改权限               |
| cat      | 打印文件内容           |
| udhcpc   | 自动获取网络 ip        |
| gedit    | ubuntu 的文本编辑      |
| grep     | 搜索                   |

### VIM 快捷键

`gg ` 光标移动到最下

`GG` 光标移动到最上

`v` 进入 Sisual 可视化

### 远程连接 tftp

安装 tftp, 可以联网后可以通过下载安装

```shell
git clone https://e.coding.net/weidongshan/DevelopmentEnvConf.git
cd DevelopmentEnvConf
sudo ./Configuring_ubuntu.sh
```

或者自行百度方法

### NFS 挂载

使本地的文件夹指向网络端的数据, 使本地的路径可以使用网络的资源

#### 安装

1.  下载安装 ``sudo apt-get install nfs-kernel-server nfs-common``

2.  修改配置文件 ``sudo vim /etc/exports``

例如： 

```shell 
/home/hyc/nfs_share *(rw,sync,no_root_squash)
```

3. 增加权限 777 ``chmod 777 ~/nfs_share/``

4. 重启

   sudo /etc/init.d/nfs-kernel-server restart

#### 挂载

指网络的资源映射到本地路径

将 nfs_share 挂载到 `Code/Test`

即 `Code/Test`(后者)路径指向 `nfs_share`(前者)

```c
sudo mount -o nolock,tcp 127.0.0.1:/home/hyc/nfs_share /mnt
```

#### 重启

``` shell
sudo /etc/init.d/nfs-kernel-server restartins 
```

####  取消

```c
umount -a  
```

### 网络

#### 改变 IP

1.查看

(1). `ip addr ` 的缩写是 `ip a` ，可以查看网卡的 `ip`、`mac` 等，

即使网卡处于 `down` 状态，也能显示出网卡状态，但是 `ifconfig` 查看就看不到。

(2)`.ip addr show device` 查看指定网卡的信息

比如查看网卡接口的信息，就是 `ip addr show eth0`

2.增加 ip

```shell
ip addr add ip/netmask dev 接口
```

比如给 `eth0` 增加一个 `172.25.21.1/24` 地址

```shell
ip addr add 172.25.21.1/24 dev eth0
```

3.删除 ip

```shell
ip addr del ip/netmask dev 接口
```

4.清空指定网卡的所有 ip

```shell
ip addr flush dev 接口
```

5. 给网卡起别名，起别名相当于给网卡多绑定了一个 ip

用法： 比如给网卡 eth0 增加别名

```shell
ip addr add 172.25.21.1/32 dev eth0 label eth0:1
```

6.删除别名

```shell
ip addr del ip/netmask dev eth0
```

原文链接：<https://blog.csdn.net/qq_43309149/article/details/104481743>

#### 获取 IP 赋值给变量

```sh
IP=$(ip a|grep -w 'eth3'|grep 'global'|sed 's/^.*inet//g'|sed 's/[0-9][0-9].*$//g')
```

### 图形化

ubuntu 的

开机时关闭 ``sudo systemctl set-default multi-user.target``

开机时打开 ``sudo systemctl set-default graphical.target``

本质上是设置 systemctl 

### Ubuntu 下载源

改变目录下/etc/apt/sources.list 文件

**参考链接**：

清华镜像源：[*ubuntu | 镜像站使用帮助 | 清华大学开源软件镜像站 | Tsinghua Open Source Mirror*](https://mirror.tuna.tsinghua.edu.cn/help/ubuntu/)

中科大源：<https://mirrors.ustc.edu.cn/repogen/>

默认的版本不够新的话可以使用下面命令获取，

```shell
wget https://apt.kitware.com/kitware-archive.sh

sudo bash kitware-archive.sh
```

如果使用早于 22.04 的 Ubuntu 版本，则需要添加额外的存储库以满足上面列出的主要依赖项的最低版本要求。在这种情况下，请下载、检查并执行 Kitware 存档脚本，以将 Kitware APT 存储库添加到您的源列表中。 kitware-archive.sh 的详细解释可以在这里找到 kitware 第 = 方 apt 存储库

### 后台运行

```shell
开启运行
nohup [command]

## 查看后台
ps -def | grep "runoob.sh"

## 比如，chatgptwechat 的
nohup python3 app.py & tail -f nohup.out

## 命令可查看运行于后台的进程
ps -ef | grep app.py | grep -v grep 
```



### **用户**

以下命令需要管理员权限

#### 创建用户

Linux 服务器上创建账户用到 `useradd` 命名，一般常用以下命令：

创建： ``useradd -m -s /bin/bash userName``

设置密码：`` passwd userName``

#### 删除用户

```shell
userdel -r userName
```

#### 给予 sudo 权限

1.  切换到 root 用户

2.  打开 `/etc/sudoers` (需要 root 用户)

3.  再添加## Allow members of group sudo to execute any command 下面一行添加 `hyc ALL=(ALL:ALL) ALL`

4.  `:/wq!` 保存退出即可使用

#### 切换用户

``su userName``

#### 修改用户名和密码

参考链接：

<https://blog.nowcoder.net/n/525cc83df73448a0909cb2a0c286df72>

**修改密码**

1. 进入 Ubuntu，打开一个终端，输入 `sudo su` 转为 root 用户。 注意，必须先转为 root 用户！！！

2. `sudo passwd user`(user 是对应的用户名)

3. 输入新密码，确认密码。

4. 修改密码成功，重启，输入新密码进入 Ubuntu。

**修改用户名**

1. 进入 Ubuntu，打开一个终端，输入 sudo su 转为 root 用户。 注意，必须先转为 root 用户！！！

2. `gedit /etc/passwd` , 找到代表你的那一行，修改用户名为新的用户名。 注意：只修改用户名！后面的全名. 目录等不要动！

3. `gedit /etc/shadow`，找到代表你的那一行，修改用户名为新用户名

4. `gedit /etc/group`，你应该发现你的用户名在很多个组中，全部修改！

5. 修改完，保存，重启。

### crontab 定时任务

```shell
出用户 user 的 crontab
crontab -l [-u user]

#修改用户 user 的 crontab
crontab -e [-u user] ## 参数还有-r(删除),-i(有提示的删除)
```

### screen

#### 命令

```shell
screen -ls #列出桌面
screen -S 23536 -X quit #删除某终端 23536 是 id，推荐进入终端关闭
screen -r $name #进入某终端
```

#### 快捷键

所有快捷键都需要按下 ctrl + a 触发快捷键模式

`k` 关闭当前窗口

`shift + s` 上下分屏

`tab` 切换屏幕

`c` 新建终端

`x` 退出

### 自启动 Service

举例 `clash.service` 文件放在 `/etc/systemd/system/` 下

```shell
[Unit]
Description=clash daemon

[Service]
Type=simple
User=hyc
ExecStart=/usr/local/bin/clash
Restart=on-failure

[Install]
WantedBy=multi-user.target
```

基本命令

1. 查看当前已经启动的服务 `systemctl list-units`

2. 查看所有服务 `systemctl list-unit-files`

3. 运行命令 `systemctl reload` 重新加载一下配置

4. 运行命令 ` systemctl status clash.service` 查看状态

5. 运行命令 `systemctl start clash.service` 开启服务

6. 运行命令 ` systemctl enable clash.service` 设置开机启动

   举例

   ```shell
   ## 重载服务
   sudo systemctl daemon-reload
   
   ## 开机启动
   sudo systemctl enable clash
   
   ## 启动服务
   sudo systemctl start clash
   
   ## 查看服务状态
   sudo systemctl status clash
   ```

### printk

控制等级

```bash
cat /proc/sys/kernel/printk ## read
echo x x x x > /proc/sys/kernel/printk ## config
```

打印等级

| PRINTK LEVEL | NUM  | MEANING                            |
| ------------ | ---- | ---------------------------------- |
| KERN_EMERG   | 0    | system is unusable                 |
| KERN_ALERT   | 1    | action must be taken   immediately |
| KERN_CRIT    | 2    | critical conditions                |
| KERN_ERR     | 3    | error conditions                   |
| KERN_WARNING | 4    | warning conditions                 |
| KERN_NOTICE  | 5    | normal but significantcondition    |
| KERN_INFO    | 6    | informational                      |
| KERN_DEBUG   | 7    | debug-level messages               |



### 路由器

[(1 条消息) 小米路由 3G 刷 openwrt 固件_奥利奥泡泡的博客-CSDN 博客_小米路由器 3gopenwrt 固件](https://blog.csdn.net/z619193774/article/details/81507917?ops_request_misc=&request_id=&biz_id=102&utm_term=%E5%B0%8F%E7%B1%B33g%E8%B7%AF%E7%94%B1%E5%99%A8openwrt&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduweb~default-1-81507917.nonecase&spm=1018.2226.3001.4187)

#### 刷机前的准备工作

##### 刷入开发者 ROM

1.在 http://www.miwifi.com/miwifi_download.html 上下载对应的 ROM for R3G 开发版

2.在小米路由器上选择系统升级，然后选择 ROM 进行升级，等重启完成后即可。

##### 开启路由器 ssh 登录

1.先让路由器绑定账号，打开小米 WIFI app，然后需要手机和路由器一个网络，登录后即可绑定

2.[http://www.miwifi.com/miwifi_open.html 在网站中找到开启 SSH 工具，会显示 root 密码，注意这个文件每个路由器都不一样。](http://www.miwifi.com/miwifi_open.html%E5%9C%A8%E7%BD%91%E7%AB%99%E4%B8%AD%E6%89%BE%E5%88%B0%E5%BC%80%E5%90%AFSSH%E5%B7%A5%E5%85%B7%EF%BC%8C%E4%BC%9A%E6%98%BE%E7%A4%BAroot%E5%AF%86%E7%A0%81%EF%BC%8C%E6%B3%A8%E6%84%8F%E8%BF%99%E4%B8%AA%E6%96%87%E4%BB%B6%E6%AF%8F%E4%B8%AA%E8%B7%AF%E7%94%B1%E5%99%A8%E9%83%BD%E4%B8%8D%E4%B8%80%E6%A0%B7%E3%80%82)

3.请将下载的工具包 bin 文件复制到 U 盘（FAT/FAT32 格式）的根目录下，保证文件名为 miwifi_ssh.bin。

4.断开小米路由器的电源，将 U 盘插入 USB 接口。

5.按住 reset 按钮之后重新接入电源，指示灯变为黄色闪烁状态即可松开 reset 键。

6.等蓝灯亮起即可刷机完成。

7.`ssh root@192.168.1.1` 即可登录到路由器上

##### 刷入 breed

Ps: 这一步是防止路由被刷坏

1.在 <https://breed.hackpascal.net/> 下载 breed-mt7621-xiaomi-r3g.bin

2.然后将文件重命名为 breed.bin 后，上传到小米路由器，可以使用 U 盘或者使用 SCP 上传，上传到/tmp 下

3.进入到小米路由开始写入 ``breed mtd -r write /tmp/breed.bin Bootloader``

4.刷入后，机器会重新启动，按住 reset 键开机，等到路由指示灯闪烁时，松开 reset 键，然后浏览器中输入 192.168.1.1 即可进入 breed 后台。

5.备份后重启路由回到小米固件的，然后 ssh 登陆准备刷入 openwrt

#### 刷入 openwrt

1. 打开 <https://downloads.lede-project.org/snapshots/targets/ramips/mt7621/> 下载对应的文件 `mir3g-squashfs-kernel1.bin`，`mir3g-squashfs-rootfs0.bin`，`mir3g-squashfs-sysupgrade.tar`，并上传到路由器。

2. 由于我刷入了 breed，需要执行

```shell
mtd write openwrt-ramips-mt7621-mir3g-squashfs-kernel1.bin kernel0
mtd write openwrt-ramips-mt7621-mir3g-squashfs-kernel1.bin kernel1
mtd write openwrt-ramips-mt7621-mir3g-squashfs-rootfs0.bin rootfs0
reboot
```

2.如果没有刷入 breed，则需要执行

```shell
mtd write openwrt-ramips-mt7621-mir3g-squashfs-kernel1.bin kernel1
mtd write openwrt-ramips-mt7621-mir3g-squashfs-rootfs0.bin rootfs0
nvram set flag_try_sys1_failed=1
nvram commit
reboot
```

ps: 刷入固件后请勿关闭路由器，要接着执行如下步骤，否则 ssh 会失效，如果 ssh 失效，请试着重置路由器。

3. 等路由启动后，即可进行 ssh 登录，ssh root@192.168.1.1 此时 root 是没有密码的，需要执行 passwd，设置完成密码后，然后安装 luci。

```shell
opkg update
opkg install luci
opkg install luci-i18n-base-zh-cn
```

4. 浏览器登陆 192.168.1.1，在界面选择系统，备份/升级，刷写新的固件，上传 `openwrt-ramips-mt7621-mir3g-squashfs-sysupgrade.tar`，升级版本，等待启动完成后执行 3 步骤，重新安装 luci。即可完成

5. 如果重置路由器，等重启完成后 web 界面是被重置没有了，执行 ``ssh root@192.168.1.1``，设置密码重新安装 luci。

6. 如果报 Collected errors:

```shell
check_data_file_clashes: Package libubox20170601 wants to install file /lib/libubox.so
But that file is already provided by package * libubox
opkg_install_cmd: Cannot install package opkg.
opkg install luci --force-overwrite
opkg install luci-i18n-base-zh-cn
```

### 设置文件所有者

改变用户组为 hyc 的 hyc 用户, 其中 `-R` 表示递归设置。

```bash
sudo chown -R hyc:hyc $dir
```

### 动态编译

**编译步骤**

预 编 译：``gcc -E c源文件 -o 输出i目标文件；``

编译阶段：``gcc -S i源文件 -o 输出s目标文件；``

汇编阶段：``gcc -c s源文件 -o 输出o目标文件；``

链接阶段：``gcc o源文件 -o 输出可执行文件；``

动态链接库

c 语言中存在 `静态库(.a)` 和 `动态库(.so)`。

动态库也叫共享库（share object）, 在程序链接的时候只是作些标记，然后在程序开始启动运行的时候，动态地加载所需库（模块）。

特性：

- 应用程序在运行的时候需要共享库

- 共享库链接出来的可执行文件比静态库链接出来的要小得多，运行多个程序时占用内存空间比也比静态库方式链接少(因为内存中只有一份共享库代码的拷贝)

- 由于有一个动态加载的过程所以速度稍慢

- 更换动态库不需要重新编译程序，只需要更换相应的库即可

  **相关命令**

  产生动态库：`-shared -fPIC`

  使用动态库：`-l name -L.`

**用法举例**

现有两个.c 文件

```c
// main.c
#include <stdio.h>
int fun(int x);

int main()
{
    printf("hello world");
    int val = fun(3);
    printf("%d", val);
    return 0;
}
```

```c
int fun(int x)
{
    return 2 * x;
}
```

1. **制作静态链接库**

   生成动态链接库，命名必须为 libxx.so 其中，xx 为自定义名字(注意！！！windows 下的命名为 xx.dll)

```shell	
gcc fun.c -shared -fPIC -o libxx.so
```

2. **编译应用**

   编译使用动态库的文件, 下面的 xx 为上面的名字

   ```shell
   gcc main.c -o main -L . -l xx
   ```

3. **改变环境**

   此时程序无法运行，根据提示可以知道程序会去 lib 目录下查找，将 `libxx.so` 放入到 `/lib` 目录下，程序才能够正常运行

### 静态链接

静态库实际上是一些目标文件的集合，只用于链接生成可执行文件阶段。链接器会将程序中使用到函数的代码从库文件中拷贝到应用程序中，一旦链接完成生成可执行文件之后，在执行程序的时候就不需要静态库了。

特性：

- 由于每个使用静态库的应用程序都需要拷贝所用函数的代码，所以静态链接的生成的可执行文件会比较大，多个程序运行时占用内存空间比较大(每个程序在内存中都有一份重复的静态库代码)

- 由于运行的时候不用从外部动态加载额外的库了，速度会比共享库快一些

- 更换一个静态库或者修改一个静态库后，需要重新编译应用程序

**静态编译**

无需库的支持，即里面没有库，编译的时候加上-static

需要使用二进制代码进行生成静态链接。

下面过程以上面代码举例

静态链接库文件名称是 `libxx.a`，（windows 下是 `libxx.lib`)

1. 制作静态链接库

   ```shell
   gcc -c fun.c -o fun.o #生成目标文件 fun.o
   ar -crv libfun.a fun.o #生成静态链接文件 libfun.a
   ```

2. 编译应用文件

```shell
   gcc main.c -o main -static -L . -l fun #生成静态链接的可执行文件
```


3. 执行程序

   生成 main 可以在无需打包的情况下运行，无需外部添加链接文件

### 动态链接文件查看

link：https://developer.aliyun.com/article/323137

Demo.

```
LD_TRACE_LOADED_OBJECTS=1 ./get_sif_data
```

## Drives Code

### **GPIO**

#### GPIO Status

```shell
cat /sys/kernel/debug/gpio
```

#### Get GPIO ID

① 先在开发板的 `/sys/class/gpio` 目录下，找到各个 `gpiochipXXX` 目录：

![](/home/hyc/Project/StudyCode/Notes/media/image33.png)

② 然后进入某个 `gpiochipXXX` 目录，查看文件 `label` 的内容，就可以知道起始号码 XXX 对于哪组 GPIO

#### Use GPIO In Shell

这个是基于标准的 linux 内核

以引脚编号为 110 为例.

```shell
echo 110 > /sys/class/gpio/export ### gpio_request
echo in > /sys/class/gpio/gpio110/direction ### gpio_direction_input
cat /sys/class/gpio/gpio110/value ### gpio_get_value
echo 110 > /sys/class/gpio/unexport ### gpio_free
```

对于输出, 以 N 为例

```shell
echo 104> /sys/class/gpio/export
echo out > /sys/class/gpio/gpio104/direction
echo 1 > /sys/class/gpio/gpio104/value
echo 104> /sys/class/gpio/unexport
```

#### GPIO Function

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

### I2C Subsystem

#### base

**独立模块**

`module_i2c_driver` 进行 init 和 exit

**注册**

一般使用这两个函数，本质上是一样的，第二个就是对本模块的宏

`int i2c_register_driver(struct module *owner,struct i2c_driver *driver)` 

`i2c_add_driver(driver)`

**卸载**

`void i2c_del_driver(struct i2c_driver *driver);`

#### Demo

[I2CDemo](../LinuxDrive/drive/iic)

#### Data

由于接口多数是 `struct i2c_client *client`，没有直接的数据传输，因此驱动有许多数据保存为单个模块的方式

client 结构中有一个成员就是 dev, 可以使用 `of_device_get_match_data` 获取，接着使用 `dev_set_drvdata`， `dev_get_drvdata`

也可以使用已经封装好的 `i2c_set_clientdata`，`i2c_get_clientdata`

还有 dev 转换成 client，`to_i2c_client`

#### Transfer

- int i2c_master_send(const struct i2c_client *client, const char * buf, int count)

- int i2c_master_recv(const struct i2c_client *client, char * buf, int count)
- int i2c_transfer(struct i2c_adapter *adap, struct i2c_msg * msgs, int num)

详见 demo，前两个发送都是对第三个的一个封装。发送的时序是：

`start` -> `device addr` -> `data(contain reg)` -> `end`

### POLL

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



### EVENT

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

### IRQ

#### Basis

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

#### Interrupts Status

`cat /proc/interrupts`

#### Tasklet

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

#### Disable IRQs

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

#### Kernel Thread

在中断下半部时，如果处理起来十分耗时，那么可以使用 work 来实现。本质是上半部中创建工作队列

```c
int request_threaded_irq(unsigned int irq, irq_handler_t handler,
			 irq_handler_t thread_fn, unsigned long irqflags,
			 const char *devname, void *dev_id)
```

### Work Queue

以下 API 为 2.6.20 之后的内核

#### Work

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

#### Delay Work

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



### Counting Time

#### Count

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

#### Jiffies

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

### Hrtimer

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

### ErrCode（errno）

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



## Appliction Code

### I2C

参考网址

[Linux 应用开发【第十二章】I2C 编程应用开发](https://blog.51cto.com/weidongshan/4795722#1231_I2C_toolsI2C_233)

#### API

具体可以查看 <linux/i2c-dev.h>

- ioctl，会发送设备地址，也可以用来发送数据

```c
    ret = ioctl(cam_fd[bus], I2C_SLAVE_FORCE, i2c_addr);
    or
    ret = ioctl(cam_fd[bus], I2C_RDWR, (unsigned long)&data); // data is struct i2c_rdwr_ioctl_data
```

- write，直接发送数据
- read， 直接读取数据



#### I2C-Tools

- i2cdetect：用于扫描 i2c 总线上的设备，并显示地址

- i2cset：设置 i2c 设备某个寄存器的值

- i2cget：读取 i2c 设备某个寄存器的值

- i2cdump：读取某个 i2c 设备所有寄存器的值

- i2ctransfer：一次性读写多个字节

## System Operate

### DEBUG

#### KGDB

1. 内核配置中打开 KGDB
2. 启动参数添加 `kgdb=ttyS0,115200`(举例)，`waitgdb`
3. 使用交叉工具链的 gdb 或者 gdb-multiarch，加载 vmlinux，连接内核。

**QEMU**

qemu 启动参数添加 `-s -S`

#### GDB

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

### Change Kernel

#### Delect

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

#### Change

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
make modules_install ## 将编译好的模块文件安装到系统中
make install ## 更新内核
```

### Drives Install

1. **Install To Rootfs**

```
make modules_install INSTALL_MOD_PATH=xxx INSTALL_NO_SUBDIR=1 
```

Parameter

- INSTALL_MOD_PATH: rootfs path, such as `/`
- INSTALL_NO_SUBDIR: no create subdirectories, *.ko files position is at `lib/modules/xxx/`
- NSTALL_MOD_STRIP: remove debug infomation

2. **In system**

安装 `insmod xxx` or  `modprobe xxx`

卸载 `rmmod xxx` or  `modprobe -r xxx`

编译内核选项中，`-m` 表示动态模块，`-y` 表示编译进内核



 



## Device Tree

### Device Tree Restore

1. 在开发板系统目录/sys/firmware 中有 `fdt` 文件，将它拷贝到 pc 机
2. 使用 kernel 目录下 dtc 工具复原

```bash
cd ${KERNEL_DIR}
scripts/dtc/dtc -I dtb -O dts -o ~/Downloads/fdt.dts ~/Downloads/fdt
```

### Overlay

#### latest code

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

#### early code

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

#### compile

使用内核的 dtc 工具

```bash
dtc -I dts -O dtb -o xxx.dtbo xxx.dts
```

### Sign Kernel

[web link](https://blog.csdn.net/qq_40227064/article/details/124467228)

```bash
./scripts/sign-file sha256 certs/signing_key.pem certs/signing_key.x509 yuor_module_path
```

## BootImg

### disk

```bash
lsblk # 查看设备名称
fdisk /dev/sdb #进入 fdisk
d #删除所有分区
n #新建 kernel、dts 分区
	p #选择主要分区
	1 #第几块分区
	40960 #起始点
	303104 #结束点 也可以用 `+262144`
n #新建 rootfs 分区
	p
	2
	303106
	+262144 #128MB
w #保存退出
```

40960 表示 20MB，303104 是 148MB 因为是以扇区为单位，一般来说扇区大小是 512B，计算如下

（20 * 1024 * 1024）/  512 = 40960

（148 * 1024 * 1024 ）/ 512 = 303104

### uboot

```bash
sudo dd if=./uboot.bin of=/dev/sdb bs=8K seek=1
```

这个命令的意思是：

1. 将 uboot.bin 烧录进 sdb 中
2. 设置块大小为 8K，意味着每次烧写数据块的大小为 8K
3. 设置偏单位为 1，用于跳过引导扇区

！这个命令比较危险，需要谨慎使用。

### kernel

```bash
sudo mount /dev/sdb1 /mnt
sudo cp ${KERN_DIR}/arch/arm64/boot/Image /mnt/
sudo cp ${KERN_DIR}/arch/arm64/boot/dts/xxx.dtb /mnt/
sudo umount /mnt
```

### start

**加载内核**

```bash
fatload mmc 0:1 0x40200000 Image
fatload mmc 0:1 0x4fa00000 xxx.dtb
```

表示从 `fat` 存储卡 `mmc 0` 中加载第 `1` 个分区的文件 `Image` 到 `0x40200000` 地址处

从 `fat` 存储卡 `mmc 0` 中加载第 `1` 个分区的文件 `xxx.dtb` 到 `0x4fa00000` 地址处

**从内存启动**

```bash
booti 0x40200000 - 0x4fa00000
```

1. `booti` 启动 Image，

2. `0x40200000` 内核地址
3. `0x4fa00000` 内核入口地址，和前面加载的设备树对应

**保存到设备**

```bash
设置串口和波特率
setenv bootargs 'console=ttyS0,115200'
# 设置加载 image 和 dtb，并从指定位置启动
setenv bootcmd 'fatload mmc 0:1 0x40200000 Image;fatload mmc 0:1 0x4fa00000 xxx.dtb;booti 0x40200000 - 0x4fa00000'
# 保存变量
saveenv
```

### rootfs

**制作 rootfs**

- [busybox](https://zhuanlan.zhihu.com/p/510625073)

Busybox 本身包含了很了 Linux 命令，但是要编译其他程序的话需要手工下载、编译，如果它需要某些依赖库，你还需要手工下载、编译这些依赖库。
如果想做一个极简的文件系统，可以使用 Busybox 手工制作。

- [buildroot](https://zhuanlan.zhihu.com/p/311116158)

它是一个自动化程序很高的系统，可以在里面配置、编译内核，配置编译 u-boot、配置编译根文件系统。在编译某些 APP 时，它会自动去下载源码、下载它的依赖库，自动编译这些程序。
Buildroot 的语法跟一般的 Makefile 语法类似，很容易掌握。

**存入 sd**

```bash
格式化分区
sudo mkfs.ext4 /dev/sdb2
sudo mount /dev/sdb2 /mnt
sudo cp -rf ./rootf_dir/* /mnt/* #根文件目录也是一个文件夹
sudo umount /mnt
```

**uboot**

```bash
ext4ls mmc 0:2 #测试是否存在
setenv bootargs 'console=ttyS0,115200 root=/dev/mmcblk1p2 rootfstype=ext4 rootwait rw  init=/sbin/init debug panic=30'
saveenv
```

`mmcblk1p2` 可能需要修改，注意看启动系统的时候的日至挂载到的 mmc 设备名称
