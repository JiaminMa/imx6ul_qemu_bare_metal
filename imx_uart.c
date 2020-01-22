/*!
 * @author  MJm
 * @date    2020/1/22
 */
#include "imx_uart.h"

static imx_uart_t *imx_uart0;

char uart_putc(unsigned char ch)
{
    while((imx_uart0->uart_status_reg_2 & 0x00000008) == 0);
    imx_uart0->uart_tx_reg = ch;
    return ch;
}

void uart_init()
{
    imx_uart0 = (imx_uart_t *)0x02020000;

    /* shutdown CSI*/
    imx_uart0->uart_control_reg2 &= ~0x04;
    imx_uart0->uart_control_reg2 &= ~0x02;

    /* reset controller */
    imx_uart0->uart_control_reg2 &= ~0x01;
    while((imx_uart0->uart_test_reg & 0x1) == 1);
    imx_uart0->uart_control_reg2 |= 0x1;
    /* Enable the UART module */
    imx_uart0->uart_control_reg1 |= 0x00000001;
    /* Set RXDMUXSEL of UCR3 */
    imx_uart0->uart_control_reg3 |= 0x00000004;

    imx_uart0->uart_fifo_control_reg |= (0x4 << 7);

    /* Set the baud rate */
    imx_uart0->uart_brm_inc_block |= 11;
    imx_uart0->uart_brm_mod_block |= 11;
    imx_uart0->uart_control_reg2 |= 0x04;
    imx_uart0->uart_control_reg2 |= 0x02;
    imx_uart0->uart_control_reg2 |= 0x00004000;
    /* Turn SCI port back on at new BAUD rate */
    imx_uart0->uart_control_reg2 &= ~0x00000100;
    imx_uart0->uart_control_reg2 &= ~0x00000040;
    imx_uart0->uart_control_reg2 &= ~0x00000020;
}

