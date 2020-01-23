/*!
 * @author  MJm
 * @date    2020/1/22
 */
#include <imx_uart.h>
#include <imx6ul_iomuxc.h>
#include <imx_gpio.h>
#include <imx_i2c.h>
#include <at24cxx.h>

static void test_led(void);
static void test_button(void);
static void test_at24cxx(void);

static void delay()
{
    uint32_t i, j = 0;
    for (i = 0; i < 65530; i++)
        for(j = 0; j < 2000; j++);
}

void c_entry()
{
    uart_init();

    printf("hello imx6ul bare metal:%x\n");

    test_at24cxx();

    while(1) {
        //test_led();
        // test_button();
    }
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

static void test_button()
{
    uint32_t button_int_stat = 0;
    uint8_t led_status = 0;
    imx_gpio_t *gpio1 = (imx_gpio_t *)0x0209c000UL;

    iomuxc_set_mux(IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03, MUX_MODE_ALT5);
    gpio_set_output(gpio1, 3);

    iomuxc_set_mux(IOMUXC_SW_MUX_CTL_PAD_UART1_CTS, MUX_MODE_ALT5);
    gpio_set_input(gpio1, 18);
    gpio_set_int_cfg(gpio1, 18, RISING_EDGE);
    gpio_clr_int_stat(gpio1, 18);
    gpio_unmask_int(gpio1, 18);

    while (1) {

        while((button_int_stat = gpio_get_int_stat(gpio1, 18)) == 0) {
            delay();
        }

        printf("button changed\n");
        led_status = ~led_status;
        gpio_clr_int_stat(gpio1, 18);
        if (led_status == 0) {
            gpio_set_dr(gpio1, 3);
        } else {
            gpio_clr_dr(gpio1, 3);
        }
        delay();
    }
}

static void test_at24cxx()
{
    uint8_t i = 0, j = 0;
    uint8_t buf[16] = {0};

    imx_i2c_t *i2c = (imx_i2c_t *)0x021A0000UL;
    at24cxx_t at24cxx;
    at24cxx.i2c_host = (void *)i2c;    
    at24cxx.set_send_mode = i2c_set_transmit_mode;
    at24cxx.set_receive_mode = i2c_set_receive_mode;
    at24cxx.start_bus = i2c_set_master;
    at24cxx.stop_bus = i2c_set_slave;
    at24cxx.wait_bus_idle = i2c_wait_bus_grant;
    at24cxx.write_byte = i2c_write_byte;
    at24cxx.read_byte = i2c_read_byte;
    at24cxx.set_repeat_start = i2c_set_repeat_start;
    at24cxx.set_transmit_ack = i2c_enable_transmit_ack;
    at24cxx.disable_transmit_ack = i2c_disable_transmit_ack;

    /* I2C1 SCL */
    iomuxc_set_mux(IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO02, MUX_MODE_ALT0);
    /* I2C1 SDA */
    iomuxc_set_mux(IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03, MUX_MODE_ALT0);

    i2c_init(i2c);
    i2c_set_clock(i2c, 0);
    i2c_enable_int(i2c);
    i2c_enable(i2c);
    for (i = 0; i < 16; i++) {
        at24cxx_write_byte(&at24cxx, i, i);
    }

    for (i = 0; i < 16; i++) {
        j = at24cxx_read_byte(&at24cxx, i);
        printf("%s: j:%d\n", __func__, j);
    }

    at24cxx_read(&at24cxx, 0, buf, 16);
    for (i = 0; i < 16; i++) {
        printf("%s: %d\n", __func__, buf[i]);
    }
}