/*!
 * @author  MJm
 * @date    2020/1/23
 */
#include <at24cxx.h>
#include <imx6ul_iomuxc.h>

void at24cxx_write_byte(at24cxx_t *at24cxx, uint32_t addr, uint8_t data)
{   
    printf("%s addr:%x, data:%x\n", __func__, addr, data);
    at24cxx->set_send_mode(at24cxx->i2c_host);
    at24cxx->start_bus(at24cxx->i2c_host);
    at24cxx->wait_bus_idle(at24cxx->i2c_host);
    at24cxx->write_byte(at24cxx->i2c_host, 0xA0);
    at24cxx->write_byte(at24cxx->i2c_host, addr);
    at24cxx->write_byte(at24cxx->i2c_host, data);
    at24cxx->stop_bus(at24cxx->i2c_host);
}

uint8_t at24cxx_read_byte(at24cxx_t *at24cxx, uint32_t addr)
{
    uint8_t ret = 0;

    at24cxx->set_send_mode(at24cxx->i2c_host);
    at24cxx->start_bus(at24cxx->i2c_host);
    at24cxx->wait_bus_idle(at24cxx->i2c_host);

    at24cxx->write_byte(at24cxx->i2c_host, 0xA0);
    at24cxx->write_byte(at24cxx->i2c_host, addr);

    if (at24cxx->set_repeat_start != 0)
        at24cxx->set_repeat_start(at24cxx->i2c_host);

    at24cxx->set_transmit_ack(at24cxx->i2c_host);
    at24cxx->write_byte(at24cxx->i2c_host, 0xA1);

    /* dummy read to make the Host happy */
    at24cxx->set_receive_mode(at24cxx->i2c_host);
    at24cxx->read_byte(at24cxx->i2c_host);
    ret = at24cxx->read_byte(at24cxx->i2c_host);

    printf("%s addr:%x, data:%x\n", __func__, addr, ret);
    at24cxx->disable_transmit_ack(at24cxx->i2c_host);
    at24cxx->stop_bus(at24cxx->i2c_host);
    return ret;
}

void at24cxx_read(at24cxx_t *at24cxx, uint32_t addr, uint8_t *src, uint32_t len)
{

    at24cxx->set_send_mode(at24cxx->i2c_host);
    at24cxx->start_bus(at24cxx->i2c_host);
    at24cxx->wait_bus_idle(at24cxx->i2c_host);

    at24cxx->write_byte(at24cxx->i2c_host, 0xA0);
    at24cxx->write_byte(at24cxx->i2c_host, addr);

    if (at24cxx->set_repeat_start != 0)
        at24cxx->set_repeat_start(at24cxx->i2c_host);

    at24cxx->set_transmit_ack(at24cxx->i2c_host);
    at24cxx->write_byte(at24cxx->i2c_host, 0xA1);

    /* dummy read to make the Host happy */
    at24cxx->set_receive_mode(at24cxx->i2c_host);
    at24cxx->read_byte(at24cxx->i2c_host);

    while (len > 0) {
        
        if (len == 2) {
            at24cxx->disable_transmit_ack(at24cxx->i2c_host);
        }

        if (len == 1) {
            at24cxx->stop_bus(at24cxx->i2c_host);
        }
        *src++ = at24cxx->read_byte(at24cxx->i2c_host);
        len--;
    }
}

void at24cxx_init(at24cxx_t *at24cxx)
{
    imx_i2c_t *i2c = (imx_i2c_t *)0x021A0000UL;
    at24cxx->i2c_host = (void *)i2c;    
    at24cxx->set_send_mode = i2c_set_transmit_mode;
    at24cxx->set_receive_mode = i2c_set_receive_mode;
    at24cxx->start_bus = i2c_set_master;
    at24cxx->stop_bus = i2c_set_slave;
    at24cxx->wait_bus_idle = i2c_wait_bus_grant;
    at24cxx->write_byte = i2c_write_byte;
    at24cxx->read_byte = i2c_read_byte;
    at24cxx->set_repeat_start = i2c_set_repeat_start;
    at24cxx->set_transmit_ack = i2c_enable_transmit_ack;
    at24cxx->disable_transmit_ack = i2c_disable_transmit_ack;

    /* I2C1 SCL */
    iomuxc_set_mux(IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO02, MUX_MODE_ALT0);
    /* I2C1 SDA */
    iomuxc_set_mux(IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03, MUX_MODE_ALT0);

    i2c_init(i2c);
    i2c_set_clock(i2c, 0);
    i2c_enable_int(i2c);
    i2c_enable(i2c);
}
