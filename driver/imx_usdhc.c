#include <imx_usdhc.h>
#include <imx6ul_iomuxc.h>
#include <timer.h>
#include <stdbool.h>

static void usdhc_create_cmd(usdhc_cmd_t *cmd,
                             uint32_t idx,
                             uint32_t arg,
                             uint8_t xfer_type,
                             uint8_t format,
                             bool data_pres,
                             bool crc_chk,
                             bool cmd_idx_chk,
                             bool dma_en)
{
    cmd->cmd = idx;
    cmd->arg = arg;
    cmd->xfer_type = xfer_type;
    cmd->resp_format = format;
    cmd->dat_pres = data_pres;
    cmd->crc_chk = crc_chk;
    cmd->cmdidx_chk = cmd_idx_chk;
    if (dma_en) {
        cmd->blk_cnt_en_chk = true;
        cmd->blk_type = MULTIPLE_BLK;
        cmd->auto_cmd12_en = true;
    }
    else {
        cmd->blk_cnt_en_chk = false;
        cmd->blk_type = SINGLE_BLK;
        cmd->auto_cmd12_en = false;
    }

    cmd->dma_en = dma_en;
    cmd->ddr_en = false;
}


void usdhc_get_response(void *host, sd_resp_t *rsp)
{
    imx_usdhc_t *usdhc = (imx_usdhc_t *)host;
    rsp->rsp0 = usdhc->cmd_rsp0;
    rsp->rsp1 = usdhc->cmd_rsp1;
    rsp->rsp2 = usdhc->cmd_rsp2;
    rsp->rsp3 = usdhc->cmd_rsp3;
}

bool usdhc_send_command(void *host, uint8_t cmd_idx, uint32_t arg)
{
    usdhc_cmd_t cmd;
    imx_usdhc_t *usdhc = (imx_usdhc_t *)host;
    uint32_t temp;
    bool ret = true;

    USDHC_TRACE("%s: cmd:%d\n", __func__, cmd_idx);
    if (cmd_idx == CMD0) {
        usdhc_create_cmd(&cmd, CMD0, 0, READ, RESPONSE_NONE,
                         false, false, false, false);
    } else if (cmd_idx == CMD55) {
        usdhc_create_cmd(&cmd, CMD55, 0, READ, RESPONSE_48, 
                            false, true, true, false);
    } else if (cmd_idx == ACMD41) {
        usdhc_create_cmd(&cmd, ACMD41, arg, READ, RESPONSE_48,
                            false, false, false, false);
    } else if (cmd_idx == CMD2) {
        usdhc_create_cmd(&cmd, CMD2, 0, READ, RESPONSE_136,
                            false, true, false, false);
    } else if (cmd_idx == CMD3) {
        usdhc_create_cmd(&cmd, CMD3, arg, READ, RESPONSE_48, 
                            false, true, true, false);
    } else if (cmd_idx == CMD7) {
        usdhc_create_cmd(&cmd, CMD7, arg, READ, RESPONSE_48_CHECK_BUSY,
                            false, true, true, false);
    } else if (cmd_idx == CMD16) {
        usdhc_create_cmd(&cmd, CMD16, arg, READ, RESPONSE_48,
                            false, true, true, false);
    } else if (cmd_idx == CMD18) {
        usdhc_create_cmd(&cmd, CMD18, arg, READ, RESPONSE_48,
                            true, true, true, true);
    } else if (cmd_idx == CMD25) {
        usdhc_create_cmd(&cmd, CMD25, arg, WRITE, RESPONSE_48,
                            true, true, true, true);
    }

    usdhc->int_status = 0x117f01ff;     /*clear interrupt status register*/
    usdhc->int_status_en |= 0x007f013f; /*enable all the interrupt status*/

    if (cmd.dma_en == true) {
        usdhc->int_status_en |= USDHC_INT_STATUS_EN_DINTSEN_MASK;
    }

    /* wait cmd line free */
    while ((usdhc->pres_state & USDHC_PRES_STATE_CIHB_MASK) != 0);
    /* wait data line free */
    if (cmd.dat_pres == true) {
        while ((usdhc->pres_state & USDHC_PRES_STATE_CDIHB_MASK) != 0);
    }
    /* write command arugment in the Command Argument Register */
    usdhc->cmd_arg = cmd.arg;

    if (cmd.dma_en == false) {
        usdhc->prot_ctrl &= ~USDHC_PROT_CTRL_DMASEL_MASK;
    } else {
        usdhc->prot_ctrl |= (2 << USDHC_PROT_CTRL_DMASEL_SHIFT) & USDHC_PROT_CTRL_DMASEL_MASK;
    }

    temp = usdhc->mix_ctrl & 0xFFFFFFC0;
    if (cmd.dma_en == true) 
        temp |= USDHC_MIX_CTRL_DMAEN_MASK;
    else 
        temp &= ~USDHC_MIX_CTRL_DMAEN_MASK;

    if (cmd.blk_cnt_en_chk == true)
        temp |= USDHC_MIX_CTRL_BCEN_MASK;
    else
        temp &= ~USDHC_MIX_CTRL_BCEN_MASK;

    if (cmd.auto_cmd12_en == true)
        temp |= USDHC_MIX_CTRL_AC12EN_MASK;
    else
        temp &= ~USDHC_MIX_CTRL_AC12EN_MASK;
    
    if (cmd.ddr_en == true)
        temp |= USDHC_MIX_CTRL_DDREN_MASK;
    else
        temp &= ~USDHC_MIX_CTRL_DDREN_MASK;

    if (cmd.xfer_type == READ)
        temp |= USDHC_MIX_CTRL_DTDSEL_MASK;
    else
        temp &= ~USDHC_MIX_CTRL_DTDSEL_MASK;
    
    if (cmd.blk_type == MULTIPLE_BLK)
        temp |= USDHC_MIX_CTRL_MSBSEL_MASK;
    else
        temp &= ~USDHC_MIX_CTRL_MSBSEL_MASK;
    usdhc->mix_ctrl = temp;
    
    temp = usdhc->cmd_xfr_type;
    temp &= ~USDHC_CMD_XFRTYPE_RSPTYPE_MASK;
    switch (cmd.resp_format) {
    case RESPONSE_NONE:
    default:
        temp |= (0 << USDHC_CMD_XFRTYPE_RSPTYPE_SHIFT) & USDHC_CMD_XFRTYPE_RSPTYPE_MASK;
        break;
    case RESPONSE_136:
        temp |= (1 << USDHC_CMD_XFRTYPE_RSPTYPE_SHIFT) & USDHC_CMD_XFRTYPE_RSPTYPE_MASK;
        break;
    case RESPONSE_48:
        temp |= (2 << USDHC_CMD_XFRTYPE_RSPTYPE_SHIFT) & USDHC_CMD_XFRTYPE_RSPTYPE_MASK;
        break;
    case RESPONSE_48_CHECK_BUSY:
        temp |= (3 << USDHC_CMD_XFRTYPE_RSPTYPE_SHIFT) & USDHC_CMD_XFRTYPE_RSPTYPE_MASK;
        break;
    }

    if (cmd.crc_chk == true)
        temp |= USDHC_CMD_XFRTYPE_CCCEN_MASK;
    else
        temp &= ~USDHC_CMD_XFRTYPE_CCCEN_MASK;

    if (cmd.cmdidx_chk == true)
        temp |= USDHC_CMD_XFRTYPE_CICEN_MASK;
    else
        temp &= ~USDHC_CMD_XFRTYPE_CICEN_MASK;
    
    if (cmd.dat_pres == true)
        temp |= USDHC_CMD_XFRTYPE_DPSEL_MASK;
    else
        temp &= ~USDHC_CMD_XFRTYPE_DPSEL_MASK;
    
    temp &= ~USDHC_CMD_XFRTYPE_CMDINX_MASK;
    temp |= (cmd.cmd << USDHC_CMD_XFRTYPE_CMDINX_SHIFT) & USDHC_CMD_XFRTYPE_CMDINX_MASK;
    usdhc->cmd_xfr_type = temp;

    if (cmd.dma_en == false) {
        /* DMAE|CIE|CEBE|CCE|CTOE|CC */
        USDHC_TRACE("%s wait DMAE|CIE|CEBE|CCE|CTOE|CC\n", __func__);
        while ((usdhc->int_status & 0x100F0001) == 0);
    } else {
        USDHC_TRACE("%s wait DMAE|DEBE|DCE|DTOE|CIE|CEBE|CCE|CTOE|TC\n", __func__);
         /* DMAE|DEBE|DCE|DTOE|CIE|CEBE|CCE|CTOE|TC */
        while((usdhc->int_status & 0x107F0002) == 0);
    }

    /* mask all the signals */
    usdhc->int_singal_en = 0;
    
    /* check CCE or CTOE error */
    if ((usdhc->int_status & USDHC_INT_STATUS_CCE_MASK) != 0) {
        ret = false;
        USDHC_TRACE("%s Command CRC error\n", __func__);
        goto cleanup;
    }

    if ((usdhc->int_status & USDHC_INT_STATUS_CTOE_MASK) != 0) {
        ret = false;
        USDHC_TRACE("%s Command Timeout error\n", __func__);
        goto cleanup;
    }

cleanup:
    USDHC_TRACE("%s, ret:%d\n", __func__, ret);
    return ret;
}

void usdhc_set_data_width(void *host, uint8_t dtw)
{
    imx_usdhc_t *usdhc = (imx_usdhc_t *)host;
    usdhc->prot_ctrl &= ~USDHC_PROT_CTRL_DTW_MASK;
    usdhc->prot_ctrl |= (dtw << USDHC_PROT_CTRL_DTW_SHIFT) & USDHC_PROT_CTRL_DTW_MASK;
}

void usdhc_set_endian_mode(void *host, uint8_t emode)
{
    imx_usdhc_t *usdhc = (imx_usdhc_t *)host;
    usdhc->prot_ctrl &= ~USDHC_PROT_CTRL_EMODE_MASK;
    usdhc->prot_ctrl |= (emode << USDHC_PROT_CTRL_EMODE_SHIFT) & USDHC_PROT_CTRL_EMODE_MASK;
}

void usdhc_set_clock(void *host, uint8_t clk_freq)
{
    imx_usdhc_t *usdhc = (imx_usdhc_t *)host;
    /*STUB for qemu, should implement in real board*/
    usdhc->sys_ctrl |= 0xf;
}

void usdhc_initialization_active(void *host)
{
    imx_usdhc_t *usdhc = (imx_usdhc_t *)host;
    usdhc->sys_ctrl |= USDHC_SYS_CTRL_INITA_MASK;
    while ((usdhc->sys_ctrl & USDHC_SYS_CTRL_INITA_MASK) != 0)
        ;
}

bool usdhc_init(void *host)
{
    imx_usdhc_t *usdhc = (imx_usdhc_t *)host;

    USDHC_TRACE("%s: usdhc:%x\n", __func__, usdhc);
    //STUB: iomux config here
    //STUB: clock config here

    usdhc->sys_ctrl |= USDHC_SYS_CTRL_RSTA_MASK;
    while ((usdhc->sys_ctrl & USDHC_SYS_CTRL_RSTA_MASK) != 0)
        ;

    usdhc_set_data_width(usdhc, USDHC_DWT_1BIT);
    usdhc_set_endian_mode(usdhc, USDHC_EMODE_LITTLE_ENDIAN);

    USDHC_TRACE("%s: disable usdhc dll\n", __func__);
    usdhc->dll_ctrl &= ~USDHC_DLL_CTRL_DLL_CTRL_ENABLE_MASK;

    USDHC_TRACE("%s: select adma2\n", __func__);
    usdhc->prot_ctrl &= ~USDHC_PROT_CTRL_DMASEL_MASK;
    usdhc->prot_ctrl |= (2 << USDHC_PROT_CTRL_DMASEL_SHIFT) & USDHC_PROT_CTRL_DMASEL_MASK;

    usdhc_set_clock(usdhc, IDENTIFICATION_FREQ);
    usdhc_initialization_active(usdhc);

    return true;
}

bool usdhc_read_block(void *host, uint8_t *dst, uint32_t blk_idx)
{
    adma_bd_t adma_bd;
    imx_usdhc_t *usdhc = (imx_usdhc_t *)host;
    USDHC_TRACE("%s dst:%x, blk_idx:0x%x\n", __func__, dst, blk_idx);
    
    adma_bd.addr = (uint32_t)dst;
    adma_bd.len = 512;
    adma_bd.att = 0x20 | 0x02 | 0x01;

    usdhc->int_status = 0x117f01ff; /* clear all the interrupt */
    usdhc->adma_sys_addr = (uint32_t)&adma_bd;
    usdhc->blk_att = (1 << USDHC_BLKATT_BLKCNT_SHIFT) | 512;
    usdhc->wtmk_lvl = 0x00000080;

    return usdhc_send_command(usdhc, CMD18, blk_idx);
}

bool usdhc_write_block(void *host, uint8_t *src, uint32_t blk_idx)
{
    adma_bd_t adma_bd;
    imx_usdhc_t *usdhc = (imx_usdhc_t *)host;
    USDHC_TRACE("%s src:%x, blk_idx:0x%x\n", __func__, src, blk_idx);

    adma_bd.addr = (uint32_t)src;
    adma_bd.len = 512;
    adma_bd.att = 0x20 | 0x02 | 0x01;

    usdhc->int_status = 0x117f01ff;
    usdhc->adma_sys_addr = (uint32_t)&adma_bd;
    usdhc->blk_att = (1 << USDHC_BLKATT_BLKCNT_SHIFT) | 512;
    usdhc->wtmk_lvl = 0x00000080;

    return usdhc_send_command(usdhc, CMD25, blk_idx);
}