/*!
 * @author  MJm
 * @date    2020/1/22
 */
#ifndef __IMX_GPIO_H__
#define __IMX_GPIO_H__

#include <stdint.h>

#define LOW_LEVEL_SENSITIVE 0
#define HIGH_LEVEL_SENSITIVE 1
#define RISING_EDGE 2
#define FALLING_EDGE 3

typedef struct imx_gpio_tag
{

    volatile uint32_t dr;
    volatile uint32_t gdir;
    volatile uint32_t psr;
    volatile uint32_t icr1;
    volatile uint32_t icr2;
    volatile uint32_t imr;
    volatile uint32_t isr;
    volatile uint32_t edge_sel;

} imx_gpio_t;

extern void gpio_set_dr(imx_gpio_t *, uint8_t);
extern void gpio_clr_dr(imx_gpio_t *, uint8_t);
extern void gpio_set_output(imx_gpio_t *, uint8_t);
extern void gpio_set_input(imx_gpio_t *, uint8_t);
extern uint8_t gpio_get_psr(imx_gpio_t *, uint8_t);
extern void gpio_set_int_cfg(imx_gpio_t *, uint8_t , uint8_t);
extern void gpio_mask_int(imx_gpio_t *, uint8_t);
extern void gpio_unmask_int(imx_gpio_t *, uint8_t);
extern uint32_t gpio_get_int_stat(imx_gpio_t *, uint8_t);
extern void gpio_clr_int_stat(imx_gpio_t *, uint8_t);
extern void gpio_set_edge_sel(imx_gpio_t *, uint8_t);
extern void gpio_clr_edge_sel(imx_gpio_t *, uint8_t);
extern void dump_gpio(imx_gpio_t *);

#endif
