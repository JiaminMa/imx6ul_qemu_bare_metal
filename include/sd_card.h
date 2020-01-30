#ifndef __SDCARD_H__
#define __SDCARD_H__

#include <stdint.h>
#include <stdbool.h>

typedef struct sd_resp_tag {
    uint32_t rsp0;
    uint32_t rsp1;
    uint32_t rsp2;
    uint32_t rsp3;
    uint8_t resp_format;
} sd_resp_t;

typedef struct sdcard_tag{

    void *host;
    uint32_t rca;
    uint32_t ocr;
    uint32_t cid[4];
    char product_name[6];
    uint8_t major;
    uint8_t minor;
    bool (*host_init)(void *);
    bool (*send_cmd)(void *, uint8_t, uint32_t);
    void (*get_resp)(void *, sd_resp_t *);
    bool (*read_block)(void *, uint8_t *, uint32_t);
    bool (*write_block)(void *, uint8_t *, uint32_t);

} sdcard_t;

enum commands {
    CMD0 = 0,
    CMD1 = 1,
    CMD2 = 2,
    CMD3 = 3,
    CMD6 = 6,
    ACMD6 = 6,
    CMD7 = 7,
    CMD8 = 8,
    CMD9 = 9,
    CMD11 = 11,
    CMD12 = 12,
    CMD13 = 13,
    CMD16 = 16,
    CMD17 = 17,
    CMD18 = 18,
    CMD19 = 19,
    CMD25 = 25,
    ACMD41 = 41,
    CMD43 = 43,
    ACMD51 = 51,
    CMD55 = 55,
};

#define IDENTIFICATION_FREQ 0    /* 400 in kHz */
#define OPERATING_MMC_FREQ 1     /* 20000 in kHz */
#define OPERATING_SD_FREQ 2      /* 25000 in kHz */
#define OPERATING_MMC_DDR_FREQ 3 /* 25000 in kHz */
#define HIGH_MMC_FREQ 4          /* 40000 in kHz */
#define HIGH_SD_FREQ 5           /* 50000 in kHz */
#define HIGH_MMC_DDR_FREQ 6      /* 50000 in kHz */
#define SDR50_FREQ 7             /* 100000 in kHz */
#define SDR104_FREQ 8            /* 200000 in kHz */

// #define SDCARD_DEBUG
#if defined(SDCARD_DEBUG)
#define SDCARD_TRACE printf
#else
#define SDCARD_TRACE(fmt, args...)
#endif

#define SDCARD_SUCCESS                0
#define SDCARD_PARAM_NULL             1
#define SDCARD_HOST_INIT_FAILURE           2
#define SDCARD_SEND_COMMADN_FAILURE        3
#define SDCARD_RECEIVE_COMMAND_FAILURE     4
#define SDCARD_SEND_COMMAND_TIMEOUT        5
#define SDCARD_SEND_COMMAND_CRC_FAILURE    6
#define SDCARD_SEND_ACMD_FAILURE           7

extern uint32_t sdcard_init(sdcard_t *);
extern uint32_t sdcard_read_block(sdcard_t *, uint8_t *, uint32_t);
extern uint32_t sdcard_write_block(sdcard_t *, uint8_t *, uint32_t);

#endif