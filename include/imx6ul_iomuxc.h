/*!
 * @author  MJm
 * @date    2020/1/22
 */
#ifndef __IMX6UL_IOMUXC_H__
#define __IMX6UL_IOMUXC_H__

#include <stdint.h>

#define SW_MUX_CTRL_PAD_MUX_MODE_MASK   0x00000007UL
#define SW_MUX_CTRL_PAD_MUX_MODE_SHIFT  0UL

#define SW_MUX_CTRL_PAD_SION_SHIFT      4UL
#define SW_MUX_CTRL_PAD_SION_MASK       (1UL << SW_MUX_CTRL_PAD_SION_SHIFT)

#define MUX_MODE_ALT0                   0UL
#define MUX_MODE_ALT1                   1UL
#define MUX_MODE_ALT2                   2UL
#define MUX_MODE_ALT3                   3UL
#define MUX_MODE_ALT4                   4UL
#define MUX_MODE_ALT5                   5UL
#define MUX_MODE_ALT6                   6UL
#define MUX_MODE_ALT7                   7UL

#define IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO01    0x020E0060UL
#define IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO02    0x020E0064UL
#define IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03    0x020E0068UL
#define IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO05    0x020E0070UL
#define IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO06    0x020E0074UL
#define IOMUXC_SW_MUX_CTL_PAD_UART1_CTS     0x020E008CUL
#define IOMIXC_I2C1_SCL_SELECT_INPUT_DAISY  0x020E05A4UL
#define IOMIXC_I2C1_SDA_SELECT_INPUT_DAISY  0x020E05A8UL

static inline void iomuxc_set_daisy_in(uint32_t iomux_addr, uint8_t mode)
{
    *((volatile uint32_t *)iomux_addr) = 0;
    *((volatile uint32_t *)iomux_addr) = mode; 
}

static inline void iomuxc_enable_sion(uint32_t iomux_addr)
{
    *((volatile uint32_t *)iomux_addr) |= SW_MUX_CTRL_PAD_SION_MASK;
}

static inline void iomuxc_disable_sion(uint32_t iomux_addr)
{
    *((volatile uint32_t *)iomux_addr) &= ~SW_MUX_CTRL_PAD_SION_MASK;
}

static inline void iomuxc_set_mux(uint32_t iomux_addr, uint8_t mux_mode)
{
    *((volatile uint32_t *)iomux_addr) &= ~SW_MUX_CTRL_PAD_MUX_MODE_MASK;
    *((volatile uint32_t *)iomux_addr) |= SW_MUX_CTRL_PAD_MUX_MODE_MASK & mux_mode;
}

#endif
