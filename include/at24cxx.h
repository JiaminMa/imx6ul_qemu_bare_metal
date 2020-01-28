/*!
 * @author  MJm
 * @date    2020/1/23
 */
#ifndef __AT24CXX_H__
#define __AT24CXX_H__

#include <imx_i2c.h>

typedef struct at24cxx_tag {
    void *i2c_host;
    void (*set_send_mode)(void *);
    void (*set_receive_mode)(void *);
    void (*start_bus)(void *);
    void (*stop_bus)(void *);
    void (*wait_bus_idle)(void *);
    void (*write_byte)(void *, uint8_t);
    uint8_t (*read_byte)(void *);
    void (*set_repeat_start)(void *);
    void (*set_transmit_ack)(void *);
    void (*disable_transmit_ack)(void *);
} at24cxx_t;

extern void at24cxx_write_byte(at24cxx_t *, uint32_t , uint8_t);
extern uint8_t at24cxx_read_byte(at24cxx_t *, uint32_t);
extern void at24cxx_read(at24cxx_t *, uint32_t , uint8_t *, uint32_t);
extern void at24cxx_init(at24cxx_t *);

#endif