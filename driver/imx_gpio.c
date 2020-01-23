/*!
 * @author  MJm
 * @date    2020/1/23
 */
#include "imx_gpio.h"
#include "imx_uart.h"

void gpio_set_dr(imx_gpio_t *gpio, uint8_t idx)
{
    gpio->dr |= (1 << idx);
}

void gpio_clr_dr(imx_gpio_t *gpio, uint8_t idx)
{
    gpio->dr &= ~(1 << idx);
}

void gpio_set_output(imx_gpio_t *gpio, uint8_t idx)
{
    gpio->gdir |= (1 << idx);
}

void gpio_set_input(imx_gpio_t *gpio, uint8_t idx)
{
    gpio->gdir &= ~(1 << idx);
}

uint8_t gpio_get_psr(imx_gpio_t *gpio, uint8_t idx)
{
    return (gpio->psr & (1 << idx));
}

void gpio_set_int_cfg(imx_gpio_t *gpio, uint8_t idx, uint8_t cfg)
{
    uint32_t shift = 0;
    uint32_t mask = 0;

    if (idx < 16)
    {
        shift = idx * 2;
        mask = 3 << (idx * 2);
        gpio->icr1 &= ~mask;
        gpio->icr1 |= (cfg << shift);
    }
    else
    {
        shift = (idx - 16) * 2;
        mask = 3 << ((idx - 16) * 2);
        gpio->icr2 &= ~mask;
        gpio->icr2 |= (cfg << shift);
    }
}

void gpio_mask_int(imx_gpio_t *gpio, uint8_t idx)
{
    gpio->imr &= ~(1 << idx);
}

void gpio_unmask_int(imx_gpio_t *gpio, uint8_t idx)
{
    gpio->imr |= (1 << idx);
}

uint32_t gpio_get_int_stat(imx_gpio_t *gpio, uint8_t idx)
{
    return gpio->isr & (1 << idx);
}

void gpio_clr_int_stat(imx_gpio_t *gpio, uint8_t idx)
{
    gpio->isr |= (1 << idx);
}

void gpio_set_edge_sel(imx_gpio_t *gpio, uint8_t idx)
{
    gpio->edge_sel |= (1 << idx);
}

void gpio_clr_edge_sel(imx_gpio_t *gpio, uint8_t idx)
{
    gpio->edge_sel &= ~(1 << idx);
}

void dump_gpio(imx_gpio_t *gpio)
{
    printf("%s isr:%x\n", __func__, gpio->isr);
    printf("%s icr1:%x\n", __func__, gpio->icr1);
    printf("%s icr2:%x\n", __func__, gpio->icr2);
    printf("%s dr:%x\n", __func__, gpio->dr);
    printf("%s gdir:%x\n", __func__, gpio->gdir);
    printf("%s imr:%x\n\n", __func__, gpio->imr);
}
