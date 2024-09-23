#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid = fork(); // 创建子进程

    if (pid < 0) {
        // fork失败
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // 子进程
        printf("I am the child process. My PID is %d\n", getpid());
        execlp("ls", "ls", NULL); // 执行ls命令
        perror("execlp failed"); // 如果execlp失败
        exit(EXIT_FAILURE);
    } else {
        // 主进程
        printf("I am the parent process. My PID is %d\n", getpid());
        sleep(1); // 等待子进程结束
        printf("Child process finished.\n");
    }

    return 0;
}