#include "imx_uart.h"

void c_entry()
{
    uart_init();

    printf("hello imx6ul bare metal\n");
    while(1);
}
