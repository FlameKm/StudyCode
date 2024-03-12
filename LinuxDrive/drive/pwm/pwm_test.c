#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define PWMLED_MAX_BRIGHTNESS 1000

typedef enum
{
    PWMLED_CMD_SET_BRIGHTNESS = 0x1,
    PWMLED_CMD_GET_BRIGHTNESS,
} pwmled_cmd_t;

int main(int argc, char *argv[])
{
    int fd = open("/dev/pwmled", O_RDWR);
    int brightness = 0;
    char key = 0;

    while ((key = getchar()) != 'q') {
        switch (key) {
            case '=':
            case '+':
                printf("+\n");
                brightness += brightness < PWMLED_MAX_BRIGHTNESS ? 10 : 0;
                break;
            case '-':
            case '_':
                printf("-\n");
                brightness -= brightness > 0 ? 10 : 0;
                break;
        }

        if (ioctl(fd, PWMLED_CMD_SET_BRIGHTNESS, brightness) < 0) {
            perror("ioctl");
            break;
        }
    }

    close(fd);
    return 0;
}