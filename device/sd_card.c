#include <sd_card.h>
#include <imx_uart.h>
#define NULL 0

static void sdcard_dump_response(sd_resp_t *resp)
{
#ifdef SDCARD_DEBUG
    SDCARD_TRACE("%s: rsp0:%x\n", __func__, resp->rsp0);
    SDCARD_TRACE("%s: rsp1:%x\n", __func__, resp->rsp1);
    SDCARD_TRACE("%s: rsp2:%x\n", __func__, resp->rsp2);
    SDCARD_TRACE("%s: rsp3:%x\n", __func__, resp->rsp3);
#endif
}

static uint32_t sdcard_go_idle_cmd0(sdcard_t *sdcard)
{
    return sdcard->send_cmd(sdcard->host, CMD0, 0) == true ?
            SDCARD_SUCCESS : SDCARD_SEND_COMMADN_FAILURE;
}

static uint32_t sdcard_send_cmd55(sdcard_t *sdcard)
{
    bool res = true;
    uint32_t ret = SDCARD_SUCCESS;
    sd_resp_t resp;
    res = sdcard->send_cmd(sdcard->host, CMD55, 0);
    if (res != true) {
        ret = SDCARD_SEND_COMMADN_FAILURE;
        goto cleanup;
    }

    sdcard->get_resp(sdcard->host, &resp);
    sdcard_dump_response(&resp);
    ret = SDCARD_SUCCESS;
cleanup:
    SDCARD_TRACE("%s ret:%d\n", __func__, ret);
    return ret;
}

static uint32_t sdcard_get_ocr_acmd41(sdcard_t *sdcard)
{
    bool res = true;
    uint32_t ret = SDCARD_SUCCESS;
    sd_resp_t resp;
    res = sdcard->send_cmd(sdcard->host, ACMD41, 0xff800000);
    if (res != true) {
        ret = SDCARD_SEND_COMMADN_FAILURE;
        goto cleanup;
    }

    sdcard->get_resp(sdcard->host, &resp);
    sdcard_dump_response(&resp);
    ret = SDCARD_SUCCESS;
cleanup:
    SDCARD_TRACE("%s ret:%d\n", __func__, ret);
    return ret;
}

static uint32_t sdcard_get_cid_cmd2(sdcard_t *sdcard)
{
    bool res = true;
    uint32_t ret = SDCARD_SUCCESS;
    sd_resp_t resp;
    res = sdcard->send_cmd(sdcard->host, CMD2, 0);
    if (res != true) {
        ret = SDCARD_SEND_COMMADN_FAILURE;
        goto cleanup;
    }

    sdcard->get_resp(sdcard->host, &resp);
    sdcard_dump_response(&resp);
    ret = SDCARD_SUCCESS;

    sdcard->cid[0] = resp.rsp0;
    sdcard->cid[1] = resp.rsp1;
    sdcard->cid[2] = resp.rsp2;
    sdcard->cid[3] = resp.rsp3;

    sdcard->product_name[5] = 0;
    sdcard->product_name[4] = sdcard->cid[2] & (0xff);
    sdcard->product_name[3] = (sdcard->cid[2] & (0xff << 8)) >> 8;
    sdcard->product_name[2] = (sdcard->cid[2] & (0xff << 16)) >> 16;
    sdcard->product_name[1] = (sdcard->cid[2] & (0xff << 24)) >> 24;
    sdcard->product_name[0] = sdcard->cid[3] & (0xff);
    SDCARD_TRACE("%s: sd card product name:%s\n", __func__, sdcard->product_name);
cleanup:
    SDCARD_TRACE("%s ret:%d\n", __func__, ret);
    return ret;
}

static uint32_t sdcard_set_rca_cmd3(sdcard_t *sdcard)
{
    bool res = true;
    uint32_t ret = SDCARD_SUCCESS;
    sd_resp_t resp;
    res = sdcard->send_cmd(sdcard->host, CMD3, 0);
    if (res != true) {
        ret = SDCARD_SEND_COMMADN_FAILURE;
        goto cleanup;
    }

    sdcard->get_resp(sdcard->host, &resp);
    sdcard->rca = resp.rsp0;
    sdcard_dump_response(&resp);
    ret = SDCARD_SUCCESS;
cleanup:
    SDCARD_TRACE("%s ret:%d\n", __func__, ret);
    return ret;
}

static uint32_t sdcard_select_card_cmd7(sdcard_t *sdcard)
{
    bool res = true;
    uint32_t ret = SDCARD_SUCCESS;
    sd_resp_t resp;
    res = sdcard->send_cmd(sdcard->host, CMD7, sdcard->rca);
    if (res != true) {
        ret = SDCARD_SEND_COMMADN_FAILURE;
        goto cleanup;
    }

    sdcard->get_resp(sdcard->host, &resp);
    sdcard_dump_response(&resp);
    ret = SDCARD_SUCCESS;
cleanup:
    SDCARD_TRACE("%s ret:%d\n", __func__, ret);
    return ret;
}

static uint32_t sdcard_set_blk_len_cmd16(sdcard_t *sdcard)
{
    bool res = true;
    uint32_t ret = SDCARD_SUCCESS;
    sd_resp_t resp;
    res = sdcard->send_cmd(sdcard->host, CMD16, 512);
    if (res != true) {
        ret = SDCARD_SEND_COMMADN_FAILURE;
        goto cleanup;
    }

    sdcard->get_resp(sdcard->host, &resp);
    sdcard_dump_response(&resp);
    ret = SDCARD_SUCCESS;
cleanup:
    SDCARD_TRACE("%s ret:%d\n", __func__, ret);
    return ret;
}

static uint32_t sdcard_device_init(sdcard_t *sdcard)
{
    uint32_t status = SDCARD_SUCCESS;
    SDCARD_TRACE("%s entry\n", __func__);
    
#define SD_INIT_SEQUENCE(func) if ((status = func(sdcard)) != SDCARD_SUCCESS) return status
    
    SD_INIT_SEQUENCE(sdcard_go_idle_cmd0);
    SD_INIT_SEQUENCE(sdcard_send_cmd55);
    SD_INIT_SEQUENCE(sdcard_get_ocr_acmd41);
    SD_INIT_SEQUENCE(sdcard_get_cid_cmd2);
    SD_INIT_SEQUENCE(sdcard_set_rca_cmd3);
    SD_INIT_SEQUENCE(sdcard_select_card_cmd7);
    SD_INIT_SEQUENCE(sdcard_set_blk_len_cmd16);
    return status;
}

uint32_t sdcard_init(sdcard_t *sdcard)
{
    uint32_t ret = SDCARD_SUCCESS;

    SDCARD_TRACE("%s entry\n", __func__);
    if ((sdcard == NULL) || (sdcard->host_init == NULL)) {
        ret = SDCARD_PARAM_NULL;
        goto cleanup;
    }

    if (sdcard->host_init(sdcard->host) == false) {
        ret = SDCARD_HOST_INIT_FAILURE;
        goto cleanup;
    }

    ret= sdcard_device_init(sdcard);

cleanup:
    SDCARD_TRACE("%s ret:%d\n", __func__, ret);
    return ret;
}

uint32_t sdcard_read_block(sdcard_t *sdcard, uint8_t *dst, uint32_t blk_idx)
{
    return sdcard->read_block(sdcard->host, dst, blk_idx) == true ?
                SDCARD_SUCCESS : SDCARD_SEND_COMMADN_FAILURE;
}

uint32_t sdcard_write_block(sdcard_t *sdcard, uint8_t *src, uint32_t blk_idx)
{
    return sdcard->write_block(sdcard->host, src, blk_idx) == true ?
                SDCARD_SUCCESS : SDCARD_SEND_COMMADN_FAILURE;
}