//
// Created by BlazeMiles_hyc on 2023/7/26.
//

#include <stdlib.h>
#include <string.h>
#include "com_buffer.h"
/**
 * 通信接口初始化，默认情况下为开启循环接收、中断接收模式
 * 还需要手动设置硬件层的驱动函数发送、接收、中断接收、id
 * @param com
 * @param max_size 最大缓存数据
 * @param one_size 单次接收数据
 */
void com_init(ComBuffer_t *com, uint16_t max_size, uint16_t one_size)
{
    if (com->init) {
        com->init(com);
    }
    com->ring = true;
    com->mode = COM_RECEIVE_IRQ;
    com->ready = false;
    com->rec_callback = NULL;
    com->error_callback = NULL;
    com->max_size = max_size;
    com->size = one_size;
    if (max_size != 0) {
        com->buffer = (uint8_t *) malloc(com->max_size);
    }
    memset(com->buffer, 0, com->max_size);
}

/**
 * 发送函数
 * @param com
 * @param str 需要发送的数组
 * @param size 发送数组的大小
 */
void com_transmit(ComBuffer_t *com, uint8_t *str, uint16_t size)
{
    if (com->transmit) {
        com->transmit(com, str, size);
    }
    else if (com->error_callback) {
        com->error_callback(com);
    }
}

/**
 * 开启接收，如果开启了循环模式，中断后会再次调用此函数
 * @param com
 */
void com_open_receive(ComBuffer_t *com)
{
    bool ret = false;
    // 缓冲区为0的话表示不接收
    if (com->size != 0) {
        switch (com->mode) {
            case COM_RECEIVE_IRQ:
                if (com->receive_irq != NULL) {
                    ret = com->receive_irq(com, com->buffer, com->size);
                }
                else {
                    ret = false;
                }
                break;
            case COM_RECEIVE_NORMAL:
                if (com->receive != NULL) {
                    ret = com->receive(com, com->buffer, com->size, 10);
                }
                else {
                    ret = false;
                }
                break;
        }
    }
    // 失败则产生错误回调信号
    if (ret == false && com->error_callback != NULL) {
        com->error_callback(com);
    }
}

/**
 * 中断执行此程序，需要将此终端回调放入硬件中断中，使驱动执行
 * @param com
 */
void com_irq_callback(ComBuffer_t *com)
{
    com->ready = true;
    // 产生接收成功信号
    if (com->rec_callback) {
        com->rec_callback(com);
    }
    // 在回调结束后清空缓存
    memset(com->buffer, 0, com->size);
    // 开启新一轮的接收
    if (com->ring) {
        com_open_receive(com);
    }
}

/**
 * 设置接收回调
 * @param com
 * @param fun
 */
void com_set_receive_callback(ComBuffer_t *com, void (*fun)(void *))
{
    if (fun == NULL) {
        return;
    }
    com->rec_callback = fun;
}

/**
 * 设置错误回调
 * @param com
 * @param fun
 */
void com_set_error_callback(ComBuffer_t *com, void (*fun)(void *))
{
    if (fun == NULL) {
        return;
    }
    com->error_callback = fun;
}

/**
 * 设置是否循环接收
 * @param com
 * @param ring
 */
void com_set_ring(ComBuffer_t *com, bool ring)
{
    com->ring = ring;
}

/**
 * 设置执行模式
 * @param com
 * @param mode COM_RECEIVE_NORMAL or COM_RECEIVE_IRQ
 */
void com_set_mode(ComBuffer_t *com, enum ComMode mode)
{
    com->mode = mode;
}

/**
 * 是否能够接收
 * @param com
 * @return
 */
bool com_is_ready(ComBuffer_t *com)
{
    bool ret = com->ready;
    com->ready = false;
    return ret;
}

/**
 * 获取返回数据指针
 * @param com
 * @return
 */
uint8_t *com_get_buffer_ptr(ComBuffer_t *com)
{
    return com->buffer;
}

/**
 * 默认开启，无需再次使能
 * @param com
 * @param sta
 */
void com_set_enable(ComBuffer_t *com, bool sta)
{
    if (sta == true && com->init) {
        com->init(com);
    }
    else {
        com->deinit(com);
    }
}