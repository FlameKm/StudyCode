# 管道(Pipe) 和 守护进程(Daemon)

Pipe 是一种进程间通信的方式，它可以将一个进程的输出直接作为另一个进程的输入。
Daemon 是一种在后台运行的进程，它不受终端控制，也不会受到用户登录或注销的影响。

## Will do:

will create a daemon process, and a user process, the user process will send a message to the daemon process, and the
daemon process will write message to /tmp/test_daemon.log.

## How to use:

```
user_daemon &
user_app
```

## How to stop:

```
pkill user_daemon
```