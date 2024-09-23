#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <syslog.h>

#define FIFO_NAME "/tmp/log_fifo"
#define LOG_FILE "/tmp/test_daemon.log"
#define BUFFER_SIZE 256

void write_log(const char *message)
{
    FILE *log_file = fopen(LOG_FILE, "a");
    if (log_file == NULL) {
        perror("Failed to open log file");
        exit(EXIT_FAILURE);
    }

    fprintf(log_file, "%s\n", message);
    fclose(log_file);
}

void daemonize()
{
    pid_t pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);// 父进程退出
    }

    // 创建新的会话
    if (setsid() < 0) {
        exit(EXIT_FAILURE);
    }

    // 改变工作目录
    chdir("/");

    // 关闭文件描述符
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // 重定向标准流
    open("/dev/null", O_RDONLY);
    open("/dev/null", O_WRONLY);
    open("/dev/null", O_RDWR);
}

int main()
{
    // 创建命名管道
    mkfifo(FIFO_NAME, 0666);
    daemonize();
    char buffer[BUFFER_SIZE];
    while (1) {
        // 等待读取来自FIFO的消息
        int fd = open(FIFO_NAME, O_RDONLY);
        if (fd == -1) {
            perror("Failed to open FIFO");
            exit(EXIT_FAILURE);
        }
        // 从FIFO中读取消息
        while (read(fd, buffer, sizeof(buffer)) > 0) {
            write_log(buffer);
        }
        close(fd);
    }

    unlink(FIFO_NAME); // 删除FIFO
    return EXIT_SUCCESS;
}