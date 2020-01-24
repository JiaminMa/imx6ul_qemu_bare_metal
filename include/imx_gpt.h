/*!
 * @author  MJm
 * @date    2020/1/24
 */
#ifndef __IMX_GPT_H__
#define __IMX_GPT_H__

#include <stdint.h>

typedef struct imx_gpt_tag {
    
    volatile uint32_t cr;
    volatile uint32_t pr;
    volatile uint32_t sr;
    volatile uint32_t ir;
    volatile uint32_t ocr1;
    volatile uint32_t ocr2;
    volatile uint32_t ocr3;
    volatile uint32_t icr1;
    volatile uint32_t icr2;
    volatile uint32_t cnt;

} imx_gpt_t;

#define GPT_CR_EN_SHIFT         0UL
#define GPT_CR_EN_MASK          (1UL << GPT_CR_EN_SHIFT)

#define GPT_CR_ENMOD_SHIFT      1UL
#define GPT_CR_ENMOD_MASK       (1UL << GPT_CR_ENMOD_SHIFT)

#define GPT_CR_DBGEN_SHIFT      2UL
#define GPT_CR_DBGEN_MASK       (1UL << GPT_CR_DBGEN_SHIFT)

#define GPT_CR_WAITEN_SHIFT     3UL
#define GPT_CR_WAITEN_MASK      (1UL << GPT_CR_WAITEN_SHIFT)

#define GPT_CR_DOZEEN_SHIFT     4UL
#define GPT_CR_DOZEEN_MASK      (1UL << GPT_CR_DOZEEN_SHIFT)

#define GPT_CR_STOPEN_SHIFT     5UL
#define GPT_CR_STOPEN_MASK      (1UL << GPT_CR_STOPEN_SHIFT)

#define GPT_CR_CLKSRC_SHIFT     6UL
#define GPT_CR_CLKSRC_MASK      (7UL << GPT_CR_CLKSRC_SHIFT)

#define GPT_CR_CLKSRC_NOCLK     (0UL << GPT_CR_CLKSRC_SHIFT)
#define GPT_CR_CLKSRC_PCLK      (1UL << GPT_CR_CLKSRC_SHIFT)
#define GPT_CR_CLKSRC_HFCLK     (2UL << GPT_CR_CLKSRC_SHIFT)
#define GPT_CR_CLKSRC_EXTCLK    (3UL << GPT_CR_CLKSRC_SHIFT)
#define GPT_CR_CLKSRC_LFCLK     (4UL << GPT_CR_CLKSRC_SHIFT)
#define GPT_CR_CLKSRC_24M       (5UL << GPT_CR_CLKSRC_SHIFT)

#define GPT_CR_FRR_SHIFT        9UL
#define GPT_CR_FRR_MASK         (1UL << GPT_CR_FRR_SHIFT)

#define GPT_CR_EN24M_SHIFT      10UL
#define GPT_CR_EN24M_MASK       (1UL << GPT_CR_EN24M_SHIFT)

#define GPT_CR_SWR_SHIFT        15UL
#define GPT_CR_SWR_MASK         (1UL << GPT_CR_SWR_SHIFT)

#define GPT_CR_IM1_SHIFT        16UL
#define GPT_CR_IM1_MASK         (3UL << GPT_CR_IM1_SHIFT)

#define GPT_CR_IM2_SHIFT        18UL
#define GPT_CR_IM2_MASK         (3UL << GPT_CR_IM2_SHIFT)

#define GPT_CR_OM1_SHIFT        20UL
#define GPT_CR_OM1_MASK         (7UL << GPT_CR_OM1_SHIFT)

#define GPT_CR_OM2_SHIFT        23UL
#define GPT_CR_OM2_MASK         (7UL << GPT_CR_OM3_SHIFT)

#define GPT_CR_OM3_SHIFT        26UL
#define GPT_CR_OM3_MASK         (7UL << GPT_CR_OM3_SHIFT)

#define GPT_CR_FO1_SHIFT        29UL
#define GPT_CR_FO1_MASK         (1UL << GPT_CR_FO1_SHIFT)

#define GPT_CR_FO2_SHIFT        30UL
#define GPT_CR_FO2_MASK         (1UL << GPT_CR_FO2_SHIFT)

#define GPT_CR_FO3_SHIFT        31UL
#define GPT_CR_FO3_MASK         (1UL << GPT_CR_FO3_SHIFT)

#define GPT_PR_PRESCALER_SHIFT  0UL
#define GPT_PR_PRESCALER_MASK   (0xFFFUL << GPT_PR_PRESCALER_SHIFT)

#define GPT_PR_PRESCALER24M_SHIFT   12UL
#define GPT_PR_PRESCALER24M_MASK    (0xFUL << GPT_PR_PRESCALER24M_SHIFT)

#define GPT_SR_OF1_SHIFT        0UL
#define GPT_SR_OF1_MASK         (1UL << GPT_SR_OF1_SHIFT)

#define GPT_SR_OF2_SHIFT        1UL
#define GPT_SR_OF2_MASK         (1UL << GPT_SR_OF2_SHIFT)

#define GPT_SR_OF3_SHIFT        2UL
#define GPT_SR_OF3_MASK         (1UL << GPT_SR_OF3_SHIFT)

#define GPT_SR_IF1_SHIFT        3UL
#define GPT_SR_IF1_MASK         (1UL << GPT_SR_IF1_SHIFT)

#define GPT_SR_IF2_SHIFT        4UL
#define GPT_SR_IF2_MASK         (1UL << GPT_SR_IF2_SHIFT)

#define GPT_SR_ROV_SHIFT        5UL
#define GPT_SR_ROV_MASK         (1UL << GPT_SR_ROV_SHIFT)

#define GPT_IR_OF1IE_SHIFT      0UL 
#define GPT_IR_OF1IE_MASK       (1UL << GPT_IR_OF1IE_SHIFT)

#define GPT_IR_OF2IE_SHIFT      1UL 
#define GPT_IR_OF2IE_MASK       (1UL << GPT_IR_OF2IE_SHIFT)

#define GPT_IR_OF3IE_SHIFT      2UL 
#define GPT_IR_OF3IE_MASK       (1UL << GPT_IR_OF3IE_SHIFT)

#define GPT_IR_IF1IE_SHIFT      3UL 
#define GPT_IR_IF1IE_MASK       (1UL << GPT_IR_IF1IE_SHIFT)

#define GPT_IR_IF2IE_SHIFT      4UL 
#define GPT_IR_IF2IE_MASK       (1UL << GPT_IR_IF2IE_SHIFT)

#define GPT_IR_ROVIE_SHIFT      5UL 
#define GPT_IR_ROVIE_MASK       (1UL << GPT_IR_ROVIE_SHIFT)

#endif