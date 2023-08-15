//
// Created by BlazeMiles_hyc on 2023/7/27.
//

#ifndef FOOT_PEDAL_USER_COM_UART_H_
#define FOOT_PEDAL_USER_COM_UART_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "com_buffer.h"

extern ComBuffer_t uart;

extern ComBuffer_t usb;

extern ComBuffer_t can;
#ifdef __cplusplus
}
#endif
#endif //FOOT_PEDAL_USER_COM_UART_H_
