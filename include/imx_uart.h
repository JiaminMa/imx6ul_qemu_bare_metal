#ifndef __IMX_UART_H__
#define __IMX_UART_H__

#include <stdint.h>

typedef struct {
    volatile uint32_t uart_rx_reg;
    volatile uint32_t unused_rx_regs[15];
    volatile uint32_t uart_tx_reg;
    volatile uint32_t unused_tx_regs[15];
    volatile uint32_t uart_control_reg1;
    volatile uint32_t uart_control_reg2;
    volatile uint32_t uart_control_reg3;
    volatile uint32_t uart_control_reg4;
    volatile uint32_t uart_fifo_control_reg;
    volatile uint32_t uart_status_reg_1;
    volatile uint32_t uart_status_reg_2;
    volatile uint32_t uart_esc_char_reg;
    volatile uint32_t uart_esc_tim_reg;
    volatile uint32_t uart_brm_inc_block;
    volatile uint32_t uart_brm_mod_block;
    volatile uint32_t uart_baud_rate_count_reg;
    volatile uint32_t uart_one_milli_reg;
    volatile uint32_t uart_test_reg;
} imx_uart_t;

extern char uart_putc(unsigned char);
extern void uart_init(void);
extern void printf(const char *, ...);

#endif
