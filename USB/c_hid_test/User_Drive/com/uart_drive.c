//
// Created by BlazeMiles_hyc on 2023/7/27.
//

#include "com_drive.h"
#include "usart.h"
#include "com_buffer.h"
#include "string.h"
/* 鼠标报文结构体 */
struct mouseHID_t
{
    uint8_t buttons;
    int8_t x;
    int8_t y;
    int8_t wheel;
};

static UART_HandleTypeDef *handle_uart = &huart1;

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size)
{
    com_irq_callback(&uart);
}

static void uart_transmit(ComBuffer_t *com, uint8_t *buf, uint16_t size)
{
    static uint8_t buffer[128]; //dma发送缓存
    memcpy(buffer, buf, size);
    while (handle_uart->gState != HAL_UART_STATE_READY) {
        HAL_Delay(1);
    }
    HAL_UART_Transmit_DMA(handle_uart, (uint8_t *) buffer, size);
}
static bool uart_receive(ComBuffer_t *com, uint8_t *buf, uint16_t size, uint16_t timeout)
{
    HAL_StatusTypeDef ret = HAL_UART_Receive(handle_uart, (uint8_t *) buf, size, timeout);
    return ret == HAL_OK;
}
static bool uart_receive_irq(ComBuffer_t *com, uint8_t *buf, uint16_t size)
{
    HAL_StatusTypeDef ret = HAL_UARTEx_ReceiveToIdle_IT(handle_uart, buf, size);
    return ret == HAL_OK;
}

ComBuffer_t uart = {
    .id = 1,
    .transmit = uart_transmit,
    .receive = uart_receive,
    .receive_irq = uart_receive_irq,
    .deinit = NULL
};
