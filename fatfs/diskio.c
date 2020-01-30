/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include <sd_card.h>
#include <imx_usdhc.h>

#define DEV_SD		0

static sdcard_t s_sdcard;
static bool s_is_sdcard_init;


#ifdef DISKIO_DEBUG
#define DSIKIO_TRACE printf
#else
#define DSIKIO_TRACE(fmt, args...)
#endif

static uint32_t disk_init_sdcard()
{
    imx_usdhc_t *usdhc = (imx_usdhc_t *)0x02190000;
    DSIKIO_TRACE("%s entry\n", __func__);
    s_sdcard.host = usdhc;
    s_sdcard.rca = 0x45670000;
    s_sdcard.host_init = usdhc_init;
    s_sdcard.send_cmd = usdhc_send_command;
    s_sdcard.get_resp = usdhc_get_response;
    s_sdcard.read_block = usdhc_read_block;
    s_sdcard.write_block = usdhc_write_block;
    return sdcard_init(&s_sdcard);
}

DSTATUS disk_status (
	BYTE pdrv		
)
{
	DSTATUS stat;
    switch(pdrv) {
    case DEV_SD:
        stat = s_is_sdcard_init == true ? RES_OK : STA_NOINIT;
        break;
    default:
        stat = STA_NODISK;
        break;
    }

	return stat;
}

DSTATUS disk_initialize (
	BYTE pdrv
)
{
	DSTATUS stat;

	uint32_t result;
    s_is_sdcard_init = false;
    switch(pdrv) {
    case DEV_SD:
        result = disk_init_sdcard();
        break;
    default:
        stat = STA_NODISK;
        break;
    }

    if (result != SDCARD_SUCCESS) {
        stat = STA_NOINIT;
    } else {
        s_is_sdcard_init = true;
        stat = RES_OK;
    }

    DSIKIO_TRACE("%s stat:%x\n", __func__, stat);
	return stat;
}



DRESULT disk_read (
	BYTE pdrv,
	BYTE *buff,	
	LBA_t sector,
	UINT count
)
{
    DSTATUS ret;
    uint32_t res = SDCARD_SUCCESS;
    if (pdrv > 0) {
        ret = STA_NODISK;
        goto cleanup;
    }

    DSIKIO_TRACE("%s buff:%x, sector:%x, count:%x\n", __func__,
                    buff, sector, count);
    while ((count > 0) || (res != SDCARD_SUCCESS)) {
        res = sdcard_read_block(&s_sdcard, buff, sector * 512);
        count--;
        sector++;
    }

    ret = (res == SDCARD_SUCCESS) ? RES_OK : RES_PARERR;
cleanup:
    DSIKIO_TRACE("%s ret:%x, buff[0]:%x, buff[1]:%x\n", __func__, ret, buff[0], buff[1]);
	return ret;
}


#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,
	const BYTE *buff,
	LBA_t sector,
	UINT count
)
{
    DSTATUS ret;
    uint32_t res = SDCARD_SUCCESS;
    if (pdrv > 0) {
        ret = STA_NODISK;
        goto cleanup;
    }

    DSIKIO_TRACE("%s buff:%x, sector:%x, count:%x\n", __func__,
                    buff, sector, count);
    while ((count > 0) || (res != SDCARD_SUCCESS)) {
        res = sdcard_write_block(&s_sdcard, buff, sector * 512);
        count--;
        sector++;
    }

    ret = (res == SDCARD_SUCCESS) ? RES_OK : RES_PARERR;
cleanup:
    DSIKIO_TRACE("%s ret:%x, buff[0]:%x, buff[1]:%x\n", __func__, ret, buff[0], buff[1]);
	return ret;
}

#endif


DRESULT disk_ioctl (
	BYTE pdrv,
	BYTE cmd,
	void *buff
)
{
	return RES_OK;
}

