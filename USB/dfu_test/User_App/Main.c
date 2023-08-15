//
// Created by BlazeMiles_hyc on 2023/7/26.
//

#include "Main.h"
#include <stdio.h>
#include <string.h>
#include "log.h"
#include "com_drive.h"
#include "com_buffer.h"

/**
 * 串口接收回调
 * @param p
 */
void receive_slot_uart(void *p)
{
    static uint8_t rec_buf[64];
    uint8_t *rec_buffer = ((ComBuffer_t *) p)->buffer;
    memcpy(rec_buf, rec_buffer, 64);
    logi("rx: %s", rec_buffer);
}
_Noreturn void Main()
{
    // * The com init, has uart & usb & can
    com_init(&uart, 64, 64);//uart
    com_set_receive_callback(&uart, receive_slot_uart);
    com_open_receive(&uart);

    com_transmit(&uart, (uint8_t *) "init ok", 12);
    uint8_t keyBoardDataEmpty[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    while (1) {
        HAL_Delay(200);
        logi("456");
    }
}
