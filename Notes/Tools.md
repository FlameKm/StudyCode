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

选择`fcitx 5`

打开配置添加

![image-20240116165547543](media/image-输入法配置.png)

3. 美化

https://github.com/search?q=fcitx5+theme&type=Repositories

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
   # ZSH_THEME="robbyrussell"
   # ZSH_THEME="random"
   # ZSH_THEME="refined"
   # ZSH_THEME="jtriley"
   ZSH_THEME="pygmalion-virtualenv"
   ```

4. 插件

zsh-syntax-highlighting 高亮

zsh-autosuggestions 自动补全

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

```bash
```

# VSCode

## clangd

This is setting.json in vscode

```json
    "clangd.arguments": [
        "--compile-commands-dir=${workspaceFolder}/build" // config clangd working dir 
        "--query-driver=/opt/gcc-ubuntu-9.3.0-2020.03-x86_64-aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc" // config gcc version
    ],
   "clangd.fallbackFlags": [
        "-I/home/hyc/Project/X3MKernel_RCID/boot/kernel/include" // add include dir
    ],
```



