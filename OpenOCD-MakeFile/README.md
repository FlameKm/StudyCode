## 环境
需要配置相关环境
- arm-none-eabi-gdb
- make
- openocd
- stm32cubemx

## MAKE

## BUILD
``` json
task
        {
            "label": "build",
            "type": "shell",
            "command": "make",
            "args": [],
            "group": {
                "kind": "test",
                "isDefault": true
            },
            "problemMatcher": []
        },
```
## DOWNLOAD
``` json
task
        {
            "label": "download",
            "type": "shell",
            "command": "openocd",
            "args": [
                "-f",
                "openocd.cfg",
                "-c",
                "program build/HELLO.elf verify reset exit"
            ],
            "group": "build"
        }
```
## debug
launch.json
插件： Cortex-Debug
``` json
        {
            "cwd": "${workspaceRoot}",
            "executable": "./build/HELLO.elf",
            "name": "Debug with OpenOCD",
            "request": "launch",
            "type": "cortex-debug",
            "servertype": "openocd",
            "configFiles": ["./openocd.cfg"],
            "searchDir": [],
            "runToEntryPoint": "main",
            "showDevDebugOutput": "none"
        },
```
GDB
```json
        {
            "name": "GDB",
            "cwd": "${workspaceRoot}",
            "request": "launch", 
            "type": "cppdbg",
            "program": "${workspaceRoot}/build/HELLO.elf",  // 改成自己的项目生成的elf文件路径
            "stopAtEntry": false,
            "externalConsole": false,
            "MIMode": "gdb",
            "miDebuggerPath": "arm-none-eabi-gdb.exe", 
            "targetArchitecture": "arm",   // 目标架构，改成arm
            "preLaunchTask": "build",  // 启动调试之前要执行的任务，这里配置的编译任务，每次调试之前执行一次编译
            "setupCommands": [ // 进入GDB以后，自动执行的一些命令  
                {
                    "description": "连接GDB Server",
                    "text": "target remote localhost:3333",
                    "ignoreFailures": false
                },
                {
                    "description": "选择调试文件(.elf)到gdb",
                    "text": "file E:/Project/Zephyr-study/HELLO/build/HELLO.elf",
                    "ignoreFailures": false
                },
                {
                    "description": "Halt",
                    "text": "mon halt",
                    "ignoreFailures": false
                },
                {
                    "description": "Reset MCU",
                    "text": "mon reset init",
                    "ignoreFailures": false
                },
                {
                    "description": "下载代码到MCU",
                    "text": "load",
                    "ignoreFailures": false
                } 
            ]
        }
```