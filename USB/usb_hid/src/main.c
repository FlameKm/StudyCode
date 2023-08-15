#include <stdio.h>
#include "hidapi.h"

#define PID 0x0007
#define VID 0x1111
hid_device *handle;
int main()
{
    unsigned char buf[20];
    int length;
    // hid init
    if (hid_init()) {
        return -1;
    }
    // hid open
    handle = hid_open(VID, PID, NULL);
    if (!handle) {
        printf("unable to open");
        return -1;
    }

    while (1) {
        length = hid_read(handle, buf, sizeof(buf) / sizeof(buf[0]));
        if (length < 0) {
            printf("unable to read");
            return -1;
        }
        printf("rx:");
        for (int i = 0; i < length; ++i) {
            printf("%d", buf[i]);
        }
        printf("\n");
    }

}