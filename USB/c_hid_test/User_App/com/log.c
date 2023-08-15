//
// Created by BlazeMiles_hyc on 2023/7/28.
//
#include "log.h"
#include "com_buffer.h"
#include "com_drive.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

void uart_printf(const char *fmt, ...)
{
#ifdef DEBUG
    uint8_t buffer[128 + 1];  // 数据长度
    memset(buffer, 0, 128);
    va_list arg_ptr;
    va_start(arg_ptr, fmt);
    vsnprintf((char *) buffer, 128 + 1, fmt, arg_ptr);
    com_transmit(&uart, buffer, strlen((char *) buffer));
    va_end(arg_ptr);
#endif
}