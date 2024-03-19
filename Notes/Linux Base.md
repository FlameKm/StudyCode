# Linux Base

## 开发环境

参考链接：<https://zhuanlan.zhihu.com/p/100367053>

![](/home/hyc/Project/StudyCode/Notes/media/image30.png)

## 基础命令

指的是自己用的多的命令，会随学习程度增加

| 命令     | 内容                   |
| -------- | ---------------------- |
| cd       | 切换目录               |
| ls       | 输出目录               |
| ifconfig | 输出当前网络状态       |
| vim      | vim 编辑器              |
| touch    | 新建文件               |
| pwd      | 输出当前路径的绝对路径 |
| cp       | 复制                   |
| rm       | 删除                   |
| mv       | 移动                   |
| unzip    | 解压 zip 文件            |
| chmod    | 修改权限               |
| cat      | 打印文件内容           |
| udhcpc   | 自动获取网络 ip         |
| gedit    | ubuntu 的文本编辑       |
| grep     | 搜索                   |

## VIM 快捷键

`gg ` 光标移动到最下

`GG` 光标移动到最上

`v` 进入 Sisual 可视化

## 远程连接 tftp

安装 tftp, 可以联网后可以通过下载安装

```shell
git clone https://e.coding.net/weidongshan/DevelopmentEnvConf.git
cd DevelopmentEnvConf
sudo ./Configuring_ubuntu.sh
```

或者自行百度方法

## NFS 挂载

使本地的文件夹指向网络端的数据, 使本地的路径可以使用网络的资源

### 安装

1.  下载安装 ``sudo apt-get install nfs-kernel-server nfs-common``

2.  修改配置文件 ``sudo vim /etc/exports``

例如： 

```shell 
/home/hyc/nfs_share *(rw,sync,no_root_squash)
```

3. 增加权限 777 ``chmod 777 ~/nfs_share/``

4. 重启

   sudo /etc/init.d/nfs-kernel-server restart

### 挂载

指网络的资源映射到本地路径

将 nfs_share 挂载到 `Code/Test`

即 `Code/Test`(后者)路径指向 `nfs_share`(前者)

```c
sudo mount -o nolock,tcp 127.0.0.1:/home/hyc/nfs_share /mnt
```

### 重启

``` shell
sudo /etc/init.d/nfs-kernel-server restartins 
```

###  取消

```c
umount -a  
```

## 网络

### 改变 IP

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

### 获取 IP 赋值给变量

```sh
IP=$(ip a|grep -w 'eth3'|grep 'global'|sed 's/^.*inet//g'|sed 's/[0-9][0-9].*$//g')
```

## 图形化

ubuntu 的

开机时关闭 ``sudo systemctl set-default multi-user.target``

开机时打开 ``sudo systemctl set-default graphical.target``

本质上是设置 systemctl 

## Ubuntu 下载源

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

## 后台运行

```shell
开启运行
nohup [command]

# 查看后台
ps -def | grep "runoob.sh"

# 比如，chatgptwechat 的
nohup python3 app.py & tail -f nohup.out

# 命令可查看运行于后台的进程
ps -ef | grep app.py | grep -v grep 
```



## **用户**

以下命令需要管理员权限

### 创建用户

Linux 服务器上创建账户用到 `useradd` 命名，一般常用以下命令：

创建： ``useradd -m -s /bin/bash userName``

设置密码：`` passwd userName``

### 删除用户

```shell
userdel -r userName
```

### 给予 sudo 权限

1.  切换到 root 用户

2.  打开 `/etc/sudoers` (需要 root 用户)

3.  再添加# Allow members of group sudo to execute any command 下面一行添加 `hyc ALL=(ALL:ALL) ALL`

4.  `:/wq!` 保存退出即可使用

### 切换用户

``su userName``

### 修改用户名和密码

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

## crontab 定时任务

```shell
出用户 user 的 crontab
crontab -l [-u user]

#修改用户 user 的 crontab
crontab -e [-u user] # 参数还有-r(删除),-i(有提示的删除)
```

## screen

### 命令

```shell
screen -ls #列出桌面
screen -S 23536 -X quit #删除某终端 23536 是 id，推荐进入终端关闭
screen -r $name #进入某终端
```

### 快捷键

所有快捷键都需要按下 ctrl + a 触发快捷键模式

`k` 关闭当前窗口

`shift + s` 上下分屏

`tab` 切换屏幕

`c` 新建终端

`x` 退出

## 自启动 Service

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
   # 重载服务
   sudo systemctl daemon-reload
   
   # 开机启动
   sudo systemctl enable clash
   
   # 启动服务
   sudo systemctl start clash
   
   # 查看服务状态
   sudo systemctl status clash
   ```

## printk

控制等级

```bash
cat /proc/sys/kernel/printk # read
echo x x x x > /proc/sys/kernel/printk # config
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



## 路由器

[(1 条消息) 小米路由 3G 刷 openwrt 固件_奥利奥泡泡的博客-CSDN 博客_小米路由器 3gopenwrt 固件](https://blog.csdn.net/z619193774/article/details/81507917?ops_request_misc=&request_id=&biz_id=102&utm_term=%E5%B0%8F%E7%B1%B33g%E8%B7%AF%E7%94%B1%E5%99%A8openwrt&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduweb~default-1-81507917.nonecase&spm=1018.2226.3001.4187)

### 刷机前的准备工作

#### 刷入开发者 ROM

1.在 http://www.miwifi.com/miwifi_download.html 上下载对应的 ROM for R3G 开发版

2.在小米路由器上选择系统升级，然后选择 ROM 进行升级，等重启完成后即可。

#### 开启路由器 ssh 登录

1.先让路由器绑定账号，打开小米 WIFI app，然后需要手机和路由器一个网络，登录后即可绑定

2.[http://www.miwifi.com/miwifi_open.html 在网站中找到开启 SSH 工具，会显示 root 密码，注意这个文件每个路由器都不一样。](http://www.miwifi.com/miwifi_open.html%E5%9C%A8%E7%BD%91%E7%AB%99%E4%B8%AD%E6%89%BE%E5%88%B0%E5%BC%80%E5%90%AFSSH%E5%B7%A5%E5%85%B7%EF%BC%8C%E4%BC%9A%E6%98%BE%E7%A4%BAroot%E5%AF%86%E7%A0%81%EF%BC%8C%E6%B3%A8%E6%84%8F%E8%BF%99%E4%B8%AA%E6%96%87%E4%BB%B6%E6%AF%8F%E4%B8%AA%E8%B7%AF%E7%94%B1%E5%99%A8%E9%83%BD%E4%B8%8D%E4%B8%80%E6%A0%B7%E3%80%82)

3.请将下载的工具包 bin 文件复制到 U 盘（FAT/FAT32 格式）的根目录下，保证文件名为 miwifi_ssh.bin。

4.断开小米路由器的电源，将 U 盘插入 USB 接口。

5.按住 reset 按钮之后重新接入电源，指示灯变为黄色闪烁状态即可松开 reset 键。

6.等蓝灯亮起即可刷机完成。

7.`ssh root@192.168.1.1` 即可登录到路由器上

#### 刷入 breed

Ps: 这一步是防止路由被刷坏

1.在 <https://breed.hackpascal.net/> 下载 breed-mt7621-xiaomi-r3g.bin

2.然后将文件重命名为 breed.bin 后，上传到小米路由器，可以使用 U 盘或者使用 SCP 上传，上传到/tmp 下

3.进入到小米路由开始写入 ``breed mtd -r write /tmp/breed.bin Bootloader``

4.刷入后，机器会重新启动，按住 reset 键开机，等到路由指示灯闪烁时，松开 reset 键，然后浏览器中输入 192.168.1.1 即可进入 breed 后台。

5.备份后重启路由回到小米固件的，然后 ssh 登陆准备刷入 openwrt

### 刷入 openwrt

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

## 设置文件所有者

改变用户组为 hyc 的 hyc 用户, 其中 `-R` 表示递归设置。

```bash
sudo chown -R hyc:hyc $dir
```

# 系统移植

## disk

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

## uboot

```bash
sudo dd if=./uboot.bin of=/dev/sdb bs=8K seek=1
```

这个命令的意思是：

1. 将 uboot.bin 烧录进 sdb 中
2. 设置块大小为 8K，意味着每次烧写数据块的大小为 8K
3. 设置偏单位为 1，用于跳过引导扇区

！这个命令比较危险，需要谨慎使用。

## kernel

```bash
sudo mount /dev/sdb1 /mnt
sudo cp ${KERN_DIR}/arch/arm64/boot/Image /mnt/
sudo cp ${KERN_DIR}/arch/arm64/boot/dts/xxx.dtb /mnt/
sudo umount /mnt
```

## start

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

## roots

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

# **树莓派**

## 交叉编译器

```shell
sudo apt-get install gcc-arm-linux-gnueabihf
sudo apt-get install g++-arm-linux-gnueabihf
```

## 交叉编译

<https://zhuanlan.zhihu.com/p/100367053>

### 安装

```shell
sudo apt install gcc-arm-linux-gnueabihf
```

交叉编译

### 问题

树莓派执行出现

```shell
./hello: /lib/arm-linux-gnueabihf/libc.so.6: version `GLIBC_2.34' not found (required by ./hello)
```

通过更换旧版本交叉编译器解决

旧版本下载链接

https://releases.linaro.org/components/toolchain/binaries/

## 引脚图

![IMG_256](/home/hyc/Project/StudyCode/Notes/media/image45.png)

## 打开串口

<https://blog.csdn.net/playmakerDJ/article/details/105399782>

```shell
查看设备
ls -l /dev

# 设置树莓派
sudo raspi-config
```

