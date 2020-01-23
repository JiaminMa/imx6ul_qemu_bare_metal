/*!
 * @author  MJm
 * @date    2020/1/23
 */
#include <imx_i2c.h>

void i2c_init(void *host)
{
    imx_i2c_t *i2c = (void *)host;
    i2c->i2cr = 0;
}

void i2c_set_slave_addr(void *host, uint8_t addr)
{
    I2C_DEBUG("%s, addr:%x\n", __func__, addr);
    imx_i2c_t *i2c = (void *)host;
    i2c->i2dr &= ~I2C_IADR_ADR_MASK;
    i2c->i2dr |= (addr & I2C_IADR_ADR_MASK);
}

void i2c_set_clock(void *host, uint8_t divider_option)
{
    I2C_DEBUG("%s, clock:%x\n", __func__, divider_option);
    imx_i2c_t *i2c = (void *)host;
    i2c->ifdr &= ~I2C_IFDR_IC_MASK;
    i2c->ifdr |= ~(divider_option & I2C_IFDR_IC_MASK);
}

void i2c_disable(void *host)
{
    imx_i2c_t *i2c = (void *)host;
    i2c->i2cr &= ~I2C_ICR_IEN_MASK;
}

void i2c_enable(void *host)
{
    imx_i2c_t *i2c = (void *)host;
    i2c->i2cr |= I2C_ICR_IEN_MASK;
}

void i2c_disable_int(void *host)
{
    imx_i2c_t *i2c = (void *)host;
    i2c->i2cr &= ~I2C_ICR_IIEN_MASK;
}

void i2c_enable_int(void *host)
{
    imx_i2c_t *i2c = (void *)host;
    i2c->i2cr |= I2C_ICR_IIEN_MASK;
}

void i2c_set_master(void *host)
{
    imx_i2c_t *i2c = (void *)host;
    i2c->i2cr |= I2C_ICR_MSTA_MASK;
}

void i2c_set_slave(void *host)
{
    imx_i2c_t *i2c = (void *)host;
    i2c->i2cr &= ~I2C_ICR_MSTA_MASK;
}

void i2c_set_receive_mode(void *host)
{
    imx_i2c_t *i2c = (void *)host;
    i2c->i2cr &= ~I2C_ICR_MTX_MASK;
}

void i2c_set_transmit_mode(void *host)
{
    imx_i2c_t *i2c = (void *)host;
    i2c->i2cr |= I2C_ICR_MTX_MASK;
}

void i2c_enable_transmit_ack(void *host)
{
    imx_i2c_t *i2c = (void *)host;
    i2c->i2cr |= I2C_ICR_TXAK_MASK;
}

void i2c_disable_transmit_ack(void *host)
{
    imx_i2c_t *i2c = (void *)host;
    i2c->i2cr &= ~I2C_ICR_TXAK_MASK;
}

void i2c_set_repeat_start(void *host)
{
    imx_i2c_t *i2c = (void *)host;
    i2c->i2cr |= I2C_ICR_RSTA_MASK;
}

void i2c_unset_repeat_start(void *host)
{
    imx_i2c_t *i2c = (void *)host;
    i2c->i2cr &= ~I2C_ICR_RSTA_MASK;
}

uint32_t i2c_get_status(void *host)
{
    imx_i2c_t *i2c = (void *)host;
    return i2c->i2cr;
}

uint8_t i2c_get_data(void *host)
{
    imx_i2c_t *i2c = (void *)host;
    return (i2c->i2dr & I2C_I2DR_DATA_MASK);
}

void i2c_set_data(void *host, uint8_t data)
{
    imx_i2c_t *i2c = (void *)host;
    i2c->i2dr = data & I2C_I2DR_DATA_MASK;
}

void i2c_wait_bus_grant(void *host)
{
    imx_i2c_t *i2c = (void *)host;
    while ((i2c->i2sr & I2C_ISR_IBB_MASK) == 0);
}

void i2c_write_byte(void *host, uint8_t data)
{
    imx_i2c_t *i2c = (void *)host;
    i2c_set_data(i2c, data);   

    while((i2c->i2sr & I2C_ISR_ICF_MASK) == 0);
    while((i2c->i2sr & I2C_ISR_IIF_MASK) == 0);
    while((i2c->i2sr & I2C_ISR_RXAK_MASK) != 0);
}

uint8_t i2c_read_byte(void *host)
{
    imx_i2c_t *i2c = (void *)host;
    while((i2c->i2sr & I2C_ISR_ICF_MASK) == 0);
    while((i2c->i2sr & I2C_ISR_IIF_MASK) == 0);
    return i2c_get_data(i2c);
}