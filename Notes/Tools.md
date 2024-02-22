# Ubuntu

## 输入法

https://zhuanlan.zhihu.com/p/508797663

1. 安装

```bash
sudo apt install fcitx5
```

2. 配置

```bash
im-config
```

选择 `fcitx 5`

打开配置添加

![image-20240116165547543](media/image-输入法配置.png)

3. 美化

[github]: https://github.com/search?q=fcitx5+theme&amp;type=Repositories

 

## 截图

1. 安装

    ```bash
    sudo apt install flameshot
    ```

2. 快捷截图命令，设备为快捷方式

    ```bash
    flameshot gui -p /home/hyc/Pictures/flameshot -c
    ```

## 剪切板历史记录

1. 安装

    ```bash
    sudo apt install diodon
    ```

2. 快捷键(命令)	

    ```bash
    /usr/bin/diodon
    ```

## ZSH

1. 安装

   ```bash
   sudo apt install zsh
   git clone https://github.com/ohmyzsh/oh-my-zsh.git ~/.oh-my-zsh
   cp ~/.oh-my-zsh/templates/zshrc.zsh-template ~/.zshrc
   ```

2. 设置默认

   ```bash
   sudo chsh -s /bin/zsh
   ```

3. 主题

   ```bash
   # ZSH_THEME = "robbyrussell"
   # ZSH_THEME = "random"
   # ZSH_THEME = "refined"
   # ZSH_THEME = "jtriley"
   ZSH_THEME="pygmalion-virtualenv"
   ```

4. 插件

zsh-syntax-highlighting 高亮

```bash
git clone https://github.com/zsh-users/zsh-syntax-highlighting.git ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-syntax-highlighting
```

zsh-autosuggestions 自动补全

```bash
git clone https://github.com/zsh-users/zsh-autosuggestions ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-autosuggestions
```

~/.zshrc 处添加 `plugins=(git zsh-syntax-highlighting zsh-autosuggestions)`

5. 配置习惯

   ```bash
   # close history share
   setopt no_share_history
   ```

## Meld - 比较文件差异

一个比较的软件

```bash
sudo apt install meld
```

## Terminator - 终端

```bash
sudo apt install terminator
```

右键个性化设置，有快捷命令

## Minicom - 串口

1. 安装

```bash
sudo apt install minicom
```

2. 使用

**打开**：`sudo minicom -D /dev/ttyUSB0 -b 921600`

**退出**：`CTRL + A X`

**菜单**：`CTRL + A`

## SSH

### 安装

```shell
sudo apt-get install openssh-server
```

### 登录

ssh 到指定端口 ``ssh xx user@ip``

**如果是转发的流量 加上 -p**

### 创建公钥

- 配置

``sudo vim /etc/ssh/sshd_config``

PermitRootLogin yes 改为 no

- 重启

``systemctl restart sshd.service``

- 生成公钥

ssh-keygen

- 删除旧的连接认证, 重新生成新的

``ssh-keygen -R +输入服务器的IP``

一个 key 对应一个，需要连接的对应上就行

### 使用公钥

原理：如果使用私钥 `id_rsa` 登录服务器时候，服务器会检查所登录的用户的 `~/.ssh/authorized_keys` 里搜索是否存在本地的 `id_rsa.pub` 文本，存在的话则允许登录。

```shell
mkdir .ssh
touch authorized_keys
echo id_rsa.pub >> authorized_keys
```

或者主机上执行

```bash
ssh-copy-id ${username}@${remote-server}
```

## 自动登陆

https://blog.csdn.net/deephacking/article/details/134108938

## 桌面图标

创建桌面图标文件, 以 clion 举例，需要注意的有 Exec、Icon

`sudo gedit /usr/share/applictions/clion.desktop`

```bash
[Desktop Entry]
Name=Clion
Exec=/usr/local/bin/clion --no-sandbox %U
Terminal=false
Type=Application
Icon=/usr/local/bin/icon/clion.png
Comment=clion
Categories=Application;
```

重启后加载

## Typora Plugin

地址：https://github.com/obgnail/typora_plugin

使用 **方法二** 安装

1. *[下载](https://github.com/obgnail/typora_plugin/releases/latest) 插件源码的压缩包，并解压。*
2. *进入 Typora 安装路径，找到包含 `window.html` 的文件夹 A（一般是 `Typora/resources/window.html` 或者 `Typora/resources/app/window.html`，推荐使用 everything 找一下）*
3. *打开文件夹 A，将源码的 plugin 文件夹粘贴进该文件夹下。*
4. 打开文件 `A/window.html`。搜索文件内容 `<script src="./app/window/frame.js" defer="defer"></script>` 或者 `<script src="./appsrc/window/frame.js" defer="defer"></script>`，并在后面加入 `<script src="./plugin/index.js" defer="defer"></script>` 保存。
5. *验证：重启 Typora，在正文区域点击鼠标右键，弹出右键菜单栏，如果能看到 `常用插件` 栏目，说明一切顺利。*

ubuntu 的 apt 默认安装的位置为 /usr/share/typora/

```bash 
git clone https://github.com/obgnail/typora_plugin.git
sudo cp -r typora_plugin/plugin /usr/share/typora/resources/
sudo gedit /usr/share/typora/resources/window.html
```


## Balena Etcher

一款烧写镜像的软件

下载地址:

[github]:(https://github.com/balena-io/etcher/releases)


# VSCode

## clangd

### Setting.json

This is setting.json in vscode

```json
    "clangd.arguments": [
        "--compile-commands-dir=${workspaceFolder}/build" // config clangd working dir 
        "--query-driver=/opt/gcc-ubuntu-9.3.0-2020.03-x86_64-aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc", // config gcc version
        "--conpletion-style=detailed", // 显示完整的函数参数类型
        "--header-insertion=never" // disable auto import include
    ],
   "clangd.fallbackFlags": [
        "-I/home/hyc/Project/X3MKernel_RCID/boot/kernel/include" // add include dir
    ],
```

### .clangd

In drivers develop, need create `.clangd` like below in work directory,  because there parameters are not supported by clangd.

```bash
CompileFlags:
  Remove: [-fconserve-stack, -fno-allow-store-data-races, -fno-ipa-sra, -fno-var-tracking-assignments, -mpc-relative-literal-loads, -march=*, -mabi=*]
```

### clang-format

clangd 自带clang-format 程序，使用.clang-format配置文件即可格式化程序，或者另外安装此拓展。

## GDB

需要 C/C++插件和 GDB DEBUG 插件

```json
    "configurations": [
        {
            "name": "kernel-debug",
            "type": "cppdbg",
            "request": "launch",
            "miDebuggerServerAddress": "127.0.0.1:1234", //gdb远程连接
            "miDebuggerPath": "aarch64-linux-gnu-gdb", // 调试器路径
            "program": "${workspaceFolder}/vmlinux", // 可执行文件
            "args": [], // 附加参数
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}", // 工作目录
            "environment": [],
            "externalConsole": false,
            "logging": {
                "engineLogging": false
            },
            "MIMode": "gdb",
        }
    ]
```

# ToolChain

## Ninja

使用 `cmake` 生成 `Ninja` 工程文件

```shell
cmake -G Ninja ..
```

使用 ninja 编译工程，会去检索当前目录下的 build.ninja 去构建

```shell
ninja
```

## CMake

### 交叉编译

参考链接：https://cmake.org/cmake/help/v3.25/manual/cmake.1.html#options

<https://zhuanlan.zhihu.com/p/100367053>

### 构建静态库和动态库

假设目录结构是

```shell
.
├── build
├── CMakeLists.txt
└── lib
├── CMakeLists.txt
├── hello.cpp
└── hello.h
```

其中 hello 为一个简单的库

**外部 cmake 为**

```cmake
PROJECT(HELLO)
ADD_SUBDIRECTORY(lib bin) #lib为包含目录，bin为构建生成的目录
```

**内部 cmake 为**

```cmake
SET(LIBHELLO_SRC hello.cpp)
ADD_LIBRARY(hello SHARED ${LIBHELLO_SRC})
```

此时直接执行 cmake 可以在 build 生成可执行程序

**解析**

```cmake
# - hello：就是正常的库名，⽣成的名字前⾯会加上lib，最终产⽣的⽂件是libhello.so
# - SHARED，动态库 STATIC，静态库
# - ${LIBHELLO_SRC} ：源⽂件
ADD_LIBRARY(hello SHARED ${LIBHELLO_SRC})
```

### **添加非标准库**

```cmake
# 对应gcc的-L指定目录
link_directories(lib)
target_link_libraries(test libshow.a)
```

## **OpenOCD**

**vscode 使用 openocd 的方法链接：**

[**https://www.jianshu.com/p/ca26b2227a58**](https://www.jianshu.com/p/ca26b2227a58)

### 开启

```shell
openocd -f openocd.cfg
```

![](media/image49.png)

openocd 会默认在当前路径搜索 `openocd.cfg` 如果有的话会执行此程序，除非使用命令-f 指定。

### 进入后台

比如使用 MobaXterm

![](media/image50.png)

### 烧录

在后台输入此命令即可烧录

```shell
program build/HELLO.elf verify reset
exit
```

或者输入

```shell
openocd -f openocd.cfg -c 'program build/HELLO.elf reset exit'
```

注意：这个方法需要指定 `cfg`，也就是前面的 `-f openocd.cfg`，这个必须要被包含，否则会导致初始化失败

### 更简单的方法（本地）

openocd.cfg 配置文件里面能够写入执行程序，比如 reset run shutdown 等

值得注意的是，如果使用这个方法并写入 `shutdown`，只是烧录程序，没办法调试。

比如烧录程序的例子

```shell
择 cmsis-dap
adapter driver cmsis-dap

#swd 模式
transport select swd

source [find target/stm32f1x.cfg]

#10M xk
adapter speed 10000

program build/HELLO.elf

reset run

shutdown
```

## MakeFile

参考链接：<https://www.cnblogs.com/QG-whz/p/5461110.html>

命令：

| 指令                                                        | 内容原文                                          | 意思                                        |
| ----------------------------------------------------------- | ------------------------------------------------- | ------------------------------------------- |
| -b, -m                                                      | Ignored for compatibility.                        | 为了兼容性而忽略。                          |
| -B, --always-make                                           | Unconditionally make all targets.                 | 无条件地构建所有目标。                      |
| -C DIRECTORY                                                | Change to DIRECTORY before doing anything.        | 在执行任何操作之前切换到 DIRECTORY 目录。     |
| -d                                                          | Print lots of debugging information.              | 打印大量调试信息。                          |
| --debug [= FLAGS]                                             | Print various types of debugging information.     | 打印各种类型的调试信息。                    |
| -e, --environment-overrides                                 | Environment variables override makefiles.         | 环境变量覆盖 makefiles 中的定义。             |
| -f FILE, --file = FILE, --makefile = FILE                       | Read FILE as a makefile.                          | 以 FILE 作为 makefile 读取。                    |
| -h, --help                                                  | Print this message and exit.                      | 打印此消息并退出。                          |
| -i, --ignore-errors                                         | Ignore errors from commands.                      | 忽略命令中的错误。                          |
| -I DIRECTORY                                                | Search DIRECTORY for included makefiles.          | 在 DIRECTORY 中搜索包含的 makefile。           |
| -j [N], --jobs [= N]                                          | Allow N jobs at once; infinite jobs with no arg.  | 允许同时进行 N 个作业；不带参数表示无限作业。 |
| -k, --keep-going                                            | Keep going when some targets can't be made.       | 在某些目标无法构建时继续。                  |
| -l [N], --load-average [= N], --max-load [= N]                  | Don't start multiple jobs unless load is below N. | 除非负载低于 N，否则不启动多个作业。         |
| -L, --check-symlink-times                                   | Use the latest mtime between symlinks and target. | 在符号链接和目标之间使用最新的修改时间。    |
| -n, --just-print, --dry-run, --recon                        | Don't actually run any commands; just print them. | 不实际运行任何命令；仅打印命令。            |
| -o FILE, --old-file = FILE, --assume-old = FILE                 | Consider FILE to be very old and don't remake it. | 将 FILE 视为非常旧，不重新构建。              |
| -p, --print-data-base                                       | Print make's internal database.                   | 打印 make 的内部数据库。                      |
| -q, --question                                              | Run no commands; exit status says if up to date.  | 不执行任何命令；退出状态表示是否为最新。    |
| -r, --no-builtin-rules                                      | Disable the built-in implicit rules.              | 禁用内置的隐含规则。                        |
| -R, --no-builtin-variables                                  | Disable the built-in variable settings.           | 禁用内置的变量设置。                        |
| -s, --silent, --quiet                                       | Don't echo commands.                              | 不回显命令。                                |
| -S, --no-keep-going, --stop                                 | Turns off -k.                                     | 关闭-k 选项。                                |
| -t, --touch                                                 | Touch targets instead of remaking them.           | 触摸目标，而不是重新构建。                  |
| -v, --version                                               | Print the version number of make and exit.        | 打印 make 的版本号并退出。                    |
| -w, --print-directory                                       | Print the current directory.                      | 打印当前目录。                              |
| --no-print-directory                                        | Turn off -w, even if it was turned on implicitly. | 关闭-w 选项，即使它是隐含开启的。            |
| -W FILE, --what-if = FILE, --new-file = FILE, --assume-new = FILE | Consider FILE to be infinitely new.               | 将 FILE 视为无限新。                          |
| --warn-undefined-variables                                  | Warn when an undefined variable is referenced.    | 当引用未定义的变量时发出警告。              |

# Other



## DOCKER 容器

### 添加容器

举例：

```shell
docker run --restart always -d --name=OpenWRT --network macnet --privileged unifreq/openwrt-aarch64 /sbin/init
```

名字：`OpenWRT` 使用网络模式：`macnet ` 镜像名字：`unifreq/openwrt-aarch64`

### 进入容器

```shell
doker exec -it [容器名或者id] bash
```

### 退出容器

快捷键 `ctrl+p+q`

或命令 输入命令 `exit`

### 查看容器

```shell
doker ps
```

### **镜像**

```shell
拉取镜像
docker pull xxx:tag

# 镜像保存为 tar
docker save -o xxx.tar xxx:tag

# 导入本地镜像
docker load --input xxx.tar
# or
cat xxx.tar.gz | docker import - openwrt/lede

# 查看镜像
docker image ls
```

## Clang Format

Use the file named .clang-format for formatting , and the configuration file is shown below:

```bash
# Generated from CLion C/C++ Code Style settings
BasedOnStyle: LLVM                         # 基于LLVM样式设置
AccessModifierOffset: -4                   # 访问修饰符偏移量
AlignAfterOpenBracket: Align               # 对齐开放括号
AlignConsecutiveAssignments: None          # 不对齐连续赋值
AlignOperands: DontAlign                   # 不对齐操作数
AllowAllArgumentsOnNextLine: false         # 不允许所有参数在下一行
AllowAllConstructorInitializersOnNextLine: false  # 不允许所有构造函数初始化器在下一行
AllowAllParametersOfDeclarationOnNextLine: false  # 不允许声明的所有参数在下一行
AllowShortBlocksOnASingleLine: Always      # 总是允许短块在一行
AllowShortCaseLabelsOnASingleLine: true    # 允许短case标签在一行
AllowShortFunctionsOnASingleLine: All      # 允许所有短函数在一行
AllowShortIfStatementsOnASingleLine: Always  # 总是允许短if语句在一行
AllowShortLambdasOnASingleLine: All       # 允许所有短lambda表达式在一行
AllowShortLoopsOnASingleLine: true        # 允许短循环在一行
AlwaysBreakAfterReturnType: None           # 不总是在返回类型后换行
AlwaysBreakTemplateDeclarations: Yes       # 总是在模板声明后换行
BreakBeforeBraces: Custom                 # 自定义大括号前换行设置
BraceWrapping:
  AfterCaseLabel: false                    # case标签后不换行
  AfterClass: true                         # 类后换行
  AfterControlStatement: Never             # 控制语句后不换行
  AfterEnum: true                          # 枚举后换行
  AfterFunction: true                      # 函数后换行
  AfterNamespace: true                     # 命名空间后换行
  AfterUnion: true                         # 联合体后换行
  BeforeCatch: true                        # catch前换行
  BeforeElse: true                         # else前换行
  IndentBraces: false                      # 不缩进大括号
  SplitEmptyFunction: false                # 不在空函数体内换行
  SplitEmptyRecord: true                   # 在空记录内换行
BreakBeforeBinaryOperators: NonAssignment  # 在非赋值二元运算符前换行
BreakBeforeTernaryOperators: true         # 在三元运算符前换行
BreakConstructorInitializers: BeforeColon  # 在冒号前换行
BreakInheritanceList: BeforeColon          # 在冒号前换行
ColumnLimit: 0                            # 列限制为0
CompactNamespaces: false                  # 命名空间不紧凑
ContinuationIndentWidth: 4                # 续行缩进宽度为4
IndentCaseLabels: true                    # 缩进case标签
IndentPPDirectives: None                  # 不缩进预处理指令
IndentWidth: 4                            # 缩进宽度为4
KeepEmptyLinesAtTheStartOfBlocks: true    # 在块开始保留空行
MaxEmptyLinesToKeep: 2                    # 保留的最大空行数为2
NamespaceIndentation: None                # 不缩进命名空间
ObjCSpaceAfterProperty: false             # 属性后没有空格
ObjCSpaceBeforeProtocolList: true         # 协议列表前有空格
PointerAlignment: Right                   # 指针右对齐
ReflowComments: false                     # 不重新排列注释
SpaceAfterCStyleCast: false               # C风格类型转换后没有空格
SpaceAfterLogicalNot: false               # 逻辑非后没有空格
SpaceAfterTemplateKeyword: false          # 模板关键字后没有空格
SpaceBeforeAssignmentOperators: true      # 赋值运算符前有空格
SpaceBeforeCpp11BracedList: false         # C++11花括号列表前没有空格
SpaceBeforeCtorInitializerColon: true     # 构造函数初始化列表冒号前有空格
SpaceBeforeInheritanceColon: false        # 继承列表冒号前没有空格
SpaceBeforeParens: ControlStatements      # 控制语句前的括号有空格
SpaceBeforeRangeBasedForLoopColon: true   # 基于范围的for循环冒号前有空格
SpaceInEmptyParentheses: false            # 空括号内没有空格
SpacesBeforeTrailingComments: 0           # 尾注释前有0个空格
SpacesInAngles: false                     # 尖括号内没有空格
SpacesInCStyleCastParentheses: false      # C风格类型转换的括号内没有空格
SpacesInContainerLiterals: false          # 容器文字中没有空格
SpacesInParentheses: false                # 括号内没有空格
SpacesInSquareBrackets: false             # 方括号内没有空格
TabWidth: 4                               # 制表符宽度为4
UseTab: Never                             # 不使用制表符
SortIncludes: false						  # 不排序头文件 
