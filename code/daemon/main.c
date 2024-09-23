#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define FIFO_NAME "/tmp/log_fifo"
#define LOG_FILE "/tmp/test_daemon.log"

int log_message(const char *message)
{
    int fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
        return -1;
    }

    // 写入日志消息到FIFO
    write(fd, message, strlen(message) + 1);// +1 for null terminator
    close(fd);
    return 0;
}

int main()
{
    unsigned int idx = 0;
    const char *message[] = {
        "This is a log message from the app.",
        "Hello World!",
        "I love Linux.",
        "I love C Language.",
    };
    srand(time(NULL));
    idx = rand() % (sizeof(message) / sizeof(message[0]));
    if (log_message(message[idx]) == -1) {
        perror("Failed to open FIFO");
        return EXIT_FAILURE;
    }
    printf("Alerted the daemon to a new log message, Please check %s\n", LOG_FILE);
    printf("add new log:\n\t%s\n", message[idx]);
    return EXIT_SUCCESS;
}