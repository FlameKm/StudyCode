//
// Created by BlazeMiles_hyc on 2023/7/28.
//

#include "com_buffer.h"
#include "usbd_custom_hid_if.h"
//#include "usbd_hid.h"

extern USBD_HandleTypeDef hUsbDeviceFS;

static void usb_transmit(ComBuffer_t *com, uint8_t *buf, uint16_t size)
{
    // USBD_HID_SendReport(&hUsbDeviceFS, buf, size);
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, buf, size);

}
static bool usb_receive(ComBuffer_t *com, uint8_t *buf, uint16_t size, uint16_t timeout)
{
    return true;
}
static bool usb_receive_irq(ComBuffer_t *com, uint8_t *buf, uint16_t size)
{
    return true;
}

ComBuffer_t usb = {
    .transmit = usb_transmit,
    .receive = usb_receive,
    .receive_irq = usb_receive_irq,
};
