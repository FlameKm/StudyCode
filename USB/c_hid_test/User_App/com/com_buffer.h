//
// Created by BlazeMiles_hyc on 2023/7/26.
//

#ifndef FOOT_PEDAL_COM_BUFFER_H_
#define FOOT_PEDAL_COM_BUFFER_H_
#ifdef __cplusplus
extern C {
#endif
#include <stdint.h>
#include <stdbool.h>

enum ComMode
{
    COM_RECEIVE_NORMAL,
    COM_RECEIVE_IRQ
};

typedef struct ComBuffer
{
    uint16_t size;      //单次接收字节大小
    uint16_t max_size;  //缓冲区最大
    uint8_t id;         //设备ID
    bool ring;          //是否循环读取
    bool ready;         //是否准备使用数据
    enum ComMode mode;  //模式
    uint8_t *buffer;    //缓冲区

    // *硬件支持
    void (*transmit)(struct ComBuffer *, uint8_t *, uint16_t);                  //硬件发送
    bool (*receive)(struct ComBuffer *, uint8_t *, uint16_t, uint16_t);         //硬件接收
    bool (*receive_irq)(struct ComBuffer *, uint8_t *, uint16_t);               //硬件中断接收
    bool (*init)(struct ComBuffer *);
    bool (*deinit)(struct ComBuffer *);

    // *回调函数
    void (*rec_callback)(void *);
    void (*error_callback)(void *);
} ComBuffer_t;

void com_init(ComBuffer_t *com, uint16_t max_size, uint16_t one_size);
void com_transmit(ComBuffer_t *com, uint8_t *str, uint16_t size);
void com_open_receive(ComBuffer_t *com);
void com_set_receive_callback(ComBuffer_t *com, void (*fun)(void *));
void com_set_error_callback(ComBuffer_t *com, void (*fun)(void *));
void com_set_ring(ComBuffer_t *com, bool ring);
void com_set_mode(ComBuffer_t *com, enum ComMode mode);
bool com_is_ready(ComBuffer_t *com);
uint8_t *com_get_buffer_ptr(ComBuffer_t *com);
void com_set_enable(ComBuffer_t *com, bool sta);

void com_irq_callback(ComBuffer_t *com);
#ifdef __cplusplus
}
#endif
#endif //FOOT_PEDAL_COM_BUFFER_H_
