/*!
 * @author  MJm
 * @date    2020/1/23
 */
#ifndef __IMX_I2C_H__
#define __IMX_I2C_H__

#include <stdint.h>
#include <stdbool.h>
#include <imx_uart.h>

#define I2C_DEBUG printf

typedef struct imx_i2c_tag {

    volatile uint32_t iadr;     /* I2C Address Register */
    volatile uint32_t ifdr;     /* I2C Frequency Register */ 
    volatile uint32_t i2cr;     /* I2C Control Register */
    volatile uint32_t i2sr;     /* I2C Status Register */
    volatile uint32_t i2dr;     /* I2C Data Register */

} imx_i2c_t;

#define I2C_IADR_ADR_SHIFT          1UL 
#define I2C_IADR_ADR_MASK           0xFEUL

#define I2C_IFDR_IC_SHIFT           0UL
#define I2C_IFDR_IC_MASK            0x3FUL

#define I2C_ICR_RSTA_SHIFT          2UL
#define I2C_ICR_RSTA_MASK           (1UL << I2C_ICR_RSTA_SHIFT)

#define I2C_ICR_TXAK_SHIFT          3UL
#define I2C_ICR_TXAK_MASK           (1UL << I2C_ICR_TXAK_SHIFT)

#define I2C_ICR_MTX_SHIFT           4UL
#define I2C_ICR_MTX_MASK            (1UL << I2C_ICR_MTX_SHIFT)

#define I2C_ICR_MSTA_SHIFT          5UL
#define I2C_ICR_MSTA_MASK           (1UL << I2C_ICR_MSTA_SHIFT)

#define I2C_ICR_IIEN_SHIFT          6UL
#define I2C_ICR_IIEN_MASK           (1UL << I2C_ICR_IIEN_SHIFT)

#define I2C_ICR_IEN_SHIFT           7UL
#define I2C_ICR_IEN_MASK            (1UL << I2C_ICR_IEN_SHIFT) 

#define I2C_ISR_RXAK_SHIFT          0UL 
#define I2C_ISR_RXAK_MASK           (1UL << I2C_ISR_RXAK_SHIFT)

#define I2C_ISR_IIF_SHIFT           1UL
#define I2C_ISR_IIF_MASK            (1UL << I2C_ISR_IIF_SHIFT)

#define I2C_ISR_SRW_SHIFT           2UL
#define I2C_ISR_SRW_MASK            (1UL << I2C_ISR_SRW_SHIFT)

#define I2C_ISR_IAL_SHIFT           4UL
#define I2C_ISR_IAL_MASK            (1UL << I2C_ISR_IAL_SHIFT)

#define I2C_ISR_IBB_SHFIT           5UL
#define I2C_ISR_IBB_MASK            (1UL << I2C_ISR_IBB_SHFIT)

#define I2C_ISR_IAAS_SHIFT          6UL
#define I2C_ISR_IAAS_MASK           (1UL << I2C_ISR_IAAS_SHIFT)

#define I2C_ISR_ICF_SHIFT           7UL
#define I2C_ISR_ICF_MASK            (1UL << I2C_ISR_ICF_SHIFT)

#define I2C_I2DR_DATA_SHIFT         0UL
#define I2C_I2DR_DATA_MASK          0xFFUL

extern void i2c_init(void *);
extern void i2c_set_clock(void *, uint8_t);
extern void i2c_set_slave_addr(void *, uint8_t);
extern void i2c_enable(void *);
extern void i2c_disable(void *);
extern void i2c_disable_int(void *);
extern void i2c_enable_int(void *);
extern void i2c_set_master(void *);
extern void i2c_set_slave(void *);
extern void i2c_set_receive_mode(void *);
extern void i2c_set_transmit_mode(void *);
extern void i2c_enable_transmit_ack(void *);
extern void i2c_disable_transmit_ack(void *);
extern void i2c_set_repeat_start(void *);
extern void i2c_unset_repeat_start(void *);
extern uint32_t i2c_get_status(void *);
extern uint8_t i2c_get_data(void *);
extern void i2c_set_data(void *, uint8_t);
extern void i2c_wait_bus_grant(void *);
extern void i2c_write_byte(void *, uint8_t);
extern uint8_t i2c_read_byte(void *);

#endif