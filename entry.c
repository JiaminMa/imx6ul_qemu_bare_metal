/*!
 * @author  MJm
 * @date    2020/1/22
 */
#include <imx_uart.h>
#include <imx6ul_iomuxc.h>
#include <imx_gpio.h>

static void delay()
{
    uint32_t i, j = 0;
    for (i = 0; i < 65530; i++)
        for(j = 0; j < 2000; j++);
}

static void test_led()
{
    uint8_t i = 0;
    imx_gpio_t *gpio1 = (imx_gpio_t *)0x0209c000UL;

    iomuxc_set_mux(IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03, MUX_MODE_ALT5);

    gpio_set_output(gpio1, 3);

    while(1) {
        if ((i % 2) == 0)
            gpio_set_dr(gpio1, 3);
        else
            gpio_clr_dr(gpio1, 3);
        printf("%s:%d\n", __func__, i++);
        delay();
    }
}


void c_entry()
{
    uart_init();

    printf("hello imx6ul bare metal:%x\n");

    while(1) {
        test_led();
    }
}
