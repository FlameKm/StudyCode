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
| vim      | vim编辑器              |
| touch    | 新建文件               |
| pwd      | 输出当前路径的绝对路径 |
| cp       | 复制                   |
| rm       | 删除                   |
| mv       | 移动                   |
| unzip    | 解压zip文件            |
| chmod    | 修改权限               |
| cat      | 打印文件内容           |
| udhcpc   | 自动获取网络ip         |
| gedit    | ubuntu的文本编辑       |
| grep     | 搜索                   |

## VIM快捷键

`gg `光标移动到最下

`GG` 光标移动到最上

`v` 进入Sisual可视化

## 远程连接tftp

安装tftp,可以联网后可以通过下载安装

```shell
git clone https://e.coding.net/weidongshan/DevelopmentEnvConf.git
cd DevelopmentEnvConf
sudo ./Configuring_ubuntu.sh
```

或者自行百度方法

## NFS挂载

使本地的文件夹指向网络端的数据,使本地的路径可以使用网络的资源

### 安装

1.  下载安装 ``sudo apt-get install nfs-kernel-server nfs-common``

2.  修改配置文件``sudo vim /etc/exports``

例如： 

```shell 
/home/hyc/nfs_share *(rw,sync,no_root_squash)
```

3. 增加权限777``chmod 777 ~/nfs_share/``

4. 重启

   sudo /etc/init.d/nfs-kernel-server restart

### 挂载

指网络的资源映射到本地路径

将nfs_share挂载到`Code/Test`

即`Code/Test`(后者)路径指向`nfs_share`(前者)

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

### 改变IP

1.查看

(1). `ip addr `的缩写是`ip a` ，可以查看网卡的`ip`、`mac`等，

即使网卡处于`down`状态，也能显示出网卡状态，但是`ifconfig`查看就看不到。

(2)`.ip addr show device` 查看指定网卡的信息

比如查看网卡接口的信息，就是`ip addr show eth0`

2.增加ip

```shell
ip addr add ip/netmask dev 接口
```

比如给`eth0`增加一个`172.25.21.1/24` 地址

```shell
ip addr add 172.25.21.1/24 dev eth0
```

3.删除ip

```shell
ip addr del ip/netmask dev 接口
```

4.清空指定网卡的所有ip

```shell
ip addr flush dev 接口
```

5. 给网卡起别名，起别名相当于给网卡多绑定了一个ip

用法： 比如给网卡eth0增加别名

```shell
ip addr add 172.25.21.1/32 dev eth0 label eth0:1
```

6.删除别名

```shell
ip addr del ip/netmask dev eth0
```

原文链接：<https://blog.csdn.net/qq_43309149/article/details/104481743>

### 获取IP赋值给变量

```sh
IP=$(ip a|grep -w 'eth3'|grep 'global'|sed 's/^.*inet//g'|sed 's/[0-9][0-9].*$//g')
```

## 图形化

ubuntu的

开机时关闭 ``sudo systemctl set-default multi-user.target``

开机时打开 ``sudo systemctl set-default graphical.target``

本质上是设置systemctl 

## Ubuntu下载源

改变目录下/etc/apt/sources.list文件

**参考链接**：

清华镜像源：[*ubuntu | 镜像站使用帮助 | 清华大学开源软件镜像站 | Tsinghua Open Source Mirror*](https://mirror.tuna.tsinghua.edu.cn/help/ubuntu/)

中科大源：<https://mirrors.ustc.edu.cn/repogen/>

默认的版本不够新的话可以使用下面命令获取，

```shell
wget https://apt.kitware.com/kitware-archive.sh

sudo bash kitware-archive.sh
```

如果使用早于 22.04 的 Ubuntu 版本，则需要添加额外的存储库以满足上面列出的主要依赖项的最低版本要求。在这种情况下，请下载、检查并执行 Kitware 存档脚本，以将 Kitware APT 存储库添加到您的源列表中。 kitware-archive.sh的详细解释可以在这里找到kitware第=方apt存储库

## 后台运行

```shell
# 开启运行
nohup [command]

# 查看后台
ps -def | grep "runoob.sh"

# 比如，chatgptwechat的
nohup python3 app.py & tail -f nohup.out

# 命令可查看运行于后台的进程
ps -ef | grep app.py | grep -v grep 
```



## **用户**

以下命令需要管理员权限

### 创建用户

Linux服务器上创建账户用到`useradd`命名，一般常用以下命令：

创建： ``useradd -m -s /bin/bash userName``

设置密码：`` passwd userName``

### 删除用户

```shell
userdel -r userName
```

### 给予sudo权限

1.  切换到root用户

2.  打开`/etc/sudoers` (需要root用户)

3.  再添加# Allow members of group sudo to execute any command下面一行添加 `hyc ALL=(ALL:ALL) ALL`

4.  `:/wq!` 保存退出即可使用

### 切换用户

``su userName``

### 修改用户名和密码

参考链接：

<https://blog.nowcoder.net/n/525cc83df73448a0909cb2a0c286df72>

**修改密码**

1. 进入Ubuntu，打开一个终端，输入 `sudo su`转为root用户。 注意，必须先转为root用户！！！

2. `sudo passwd user`(user 是对应的用户名)

3. 输入新密码，确认密码。

4. 修改密码成功，重启，输入新密码进入Ubuntu。

**修改用户名**

1. 进入Ubuntu，打开一个终端，输入 sudo su转为root用户。 注意，必须先转为root用户！！！

2. `gedit /etc/passwd` ,找到代表你的那一行，修改用户名为新的用户名。 注意：只修改用户名！后面的全名. 目录等不要动！

3. `gedit /etc/shadow`，找到代表你的那一行，修改用户名为新用户名

4. `gedit /etc/group`，你应该发现你的用户名在很多个组中，全部修改！

5. 修改完，保存，重启。

## crontab定时任务

```shell
#列出用户user的crontab
crontab -l [-u user]

#修改用户user的crontab
crontab -e [-u user] # 参数还有-r(删除),-i(有提示的删除)
```

## screen

### 命令

```shell
screen -ls #列出桌面
screen -S 23536 -X quit #删除某终端 23536是id，推荐进入终端关闭
screen -r $name #进入某终端
```

### 快捷键

所有快捷键都需要按下ctrl + a触发快捷键模式

`k`关闭当前窗口

`shift + s`上下分屏

`tab`切换屏幕

`c` 新建终端

`x`退出

## 自启动Service

举例`clash.service`文件放在 `/etc/systemd/system/`下

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

4. 运行命令` systemctl status clash.service` 查看状态

5. 运行命令 `systemctl start clash.service` 开启服务

6. 运行命令` systemctl enable clash.service` 设置开机启动

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

## 路由器

[(1条消息) 小米路由3G刷openwrt固件_奥利奥泡泡的博客-CSDN博客_小米路由器3gopenwrt固件](https://blog.csdn.net/z619193774/article/details/81507917?ops_request_misc=&request_id=&biz_id=102&utm_term=%E5%B0%8F%E7%B1%B33g%E8%B7%AF%E7%94%B1%E5%99%A8openwrt&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduweb~default-1-81507917.nonecase&spm=1018.2226.3001.4187)

### 刷机前的准备工作

#### 刷入开发者ROM

1.在http://www.miwifi.com/miwifi_download.html上下载对应的ROM for R3G 开发版

2.在小米路由器上选择系统升级，然后选择ROM进行升级，等重启完成后即可。

#### 开启路由器ssh登录

1.先让路由器绑定账号，打开小米WIFI app，然后需要手机和路由器一个网络，登录后即可绑定

2.[http://www.miwifi.com/miwifi_open.html在网站中找到开启SSH工具，会显示root密码，注意这个文件每个路由器都不一样。](http://www.miwifi.com/miwifi_open.html%E5%9C%A8%E7%BD%91%E7%AB%99%E4%B8%AD%E6%89%BE%E5%88%B0%E5%BC%80%E5%90%AFSSH%E5%B7%A5%E5%85%B7%EF%BC%8C%E4%BC%9A%E6%98%BE%E7%A4%BAroot%E5%AF%86%E7%A0%81%EF%BC%8C%E6%B3%A8%E6%84%8F%E8%BF%99%E4%B8%AA%E6%96%87%E4%BB%B6%E6%AF%8F%E4%B8%AA%E8%B7%AF%E7%94%B1%E5%99%A8%E9%83%BD%E4%B8%8D%E4%B8%80%E6%A0%B7%E3%80%82)

3.请将下载的工具包bin文件复制到U盘（FAT/FAT32格式）的根目录下，保证文件名为miwifi_ssh.bin。

4.断开小米路由器的电源，将U盘插入USB接口。

5.按住reset按钮之后重新接入电源，指示灯变为黄色闪烁状态即可松开reset键。

6.等蓝灯亮起即可刷机完成。

7.`ssh root@192.168.1.1`即可登录到路由器上

#### 刷入breed

Ps:这一步是防止路由被刷坏

1.在<https://breed.hackpascal.net/>下载breed-mt7621-xiaomi-r3g.bin

2.然后将文件重命名为breed.bin后，上传到小米路由器，可以使用U盘或者使用SCP上传，上传到/tmp下

3.进入到小米路由开始写入``breed mtd -r write /tmp/breed.bin Bootloader``

4.刷入后，机器会重新启动，按住reset键开机，等到路由指示灯闪烁时，松开reset键，然后浏览器中输入192.168.1.1即可进入breed后台。

5.备份后重启路由回到小米固件的，然后ssh登陆准备刷入openwrt

### 刷入openwrt

1. 打开 <https://downloads.lede-project.org/snapshots/targets/ramips/mt7621/>下载对应的文件`mir3g-squashfs-kernel1.bin`，`mir3g-squashfs-rootfs0.bin`，`mir3g-squashfs-sysupgrade.tar`，并上传到路由器。

2. 由于我刷入了breed，需要执行

```shell
mtd write openwrt-ramips-mt7621-mir3g-squashfs-kernel1.bin kernel0
mtd write openwrt-ramips-mt7621-mir3g-squashfs-kernel1.bin kernel1
mtd write openwrt-ramips-mt7621-mir3g-squashfs-rootfs0.bin rootfs0
reboot
```

2.如果没有刷入breed，则需要执行

```shell
mtd write openwrt-ramips-mt7621-mir3g-squashfs-kernel1.bin kernel1
mtd write openwrt-ramips-mt7621-mir3g-squashfs-rootfs0.bin rootfs0
nvram set flag_try_sys1_failed=1
nvram commit
reboot
```

ps:刷入固件后请勿关闭路由器，要接着执行如下步骤，否则ssh会失效，如果ssh失效，请试着重置路由器。

3. 等路由启动后，即可进行ssh登录，ssh root@192.168.1.1此时root是没有密码的，需要执行passwd，设置完成密码后，然后安装luci。

```shell
opkg update
opkg install luci
opkg install luci-i18n-base-zh-cn
```

4. 浏览器登陆192.168.1.1，在界面选择系统，备份/升级，刷写新的固件，上传`openwrt-ramips-mt7621-mir3g-squashfs-sysupgrade.tar`，升级版本，等待启动完成后执行3步骤，重新安装luci。即可完成

5. 如果重置路由器，等重启完成后web界面是被重置没有了，执行``ssh root@192.168.1.1``，设置密码重新安装luci。

6. 如果报 Collected errors:

```shell
check_data_file_clashes: Package libubox20170601 wants to install file /lib/libubox.so
But that file is already provided by package * libubox
opkg_install_cmd: Cannot install package opkg.
opkg install luci --force-overwrite
opkg install luci-i18n-base-zh-cn
```

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
# 查看设备
ls -l /dev

# 设置树莓派
sudo raspi-config
```

