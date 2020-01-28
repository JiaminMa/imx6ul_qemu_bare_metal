/*
 * Author:Jiamin Ma
 *  mail: jiamin_ma@126.com
 */
#ifndef MMC_CARD_H
#define MMC_CARD_H

#include <stdint.h>
#include <stdbool.h>

#define MMC_RSP_PRESENT (1 << 0)
#define MMC_RSP_136 (1 << 1)        /* 136 bit response */
#define MMC_RSP_CRC (1 << 2)        /* expect valid crc */
#define MMC_RSP_BUSY    (1 << 3)        /* card may send busy */
#define MMC_RSP_OPCODE  (1 << 4)        /* response contains opcode */

#define MMC_CMD_MASK    (3 << 5)        /* non-SPI command type */
#define MMC_CMD_AC  (0 << 5)
#define MMC_CMD_ADTC    (1 << 5)
#define MMC_CMD_BC  (2 << 5)
#define MMC_CMD_BCR (3 << 5)

#define MMC_RSP_SPI_S1  (1 << 7)        /* one status byte */
#define MMC_RSP_SPI_S2  (1 << 8)        /* second byte */
#define MMC_RSP_SPI_B4  (1 << 9)        /* four data bytes */
#define MMC_RSP_SPI_BUSY (1 << 10)      /* card may send busy */

/*
 *  * These are the native response types, and correspond to valid bit
 *   * patterns of the above flags.  One additional valid pattern
 *    * is all zeros, which means we don't expect a response.
 *     */
#define MMC_RSP_NONE    (0)
#define MMC_RSP_R1  (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R1B (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE|MMC_RSP_BUSY)
#define MMC_RSP_R2  (MMC_RSP_PRESENT|MMC_RSP_136|MMC_RSP_CRC)
#define MMC_RSP_R3  (MMC_RSP_PRESENT)
#define MMC_RSP_R4  (MMC_RSP_PRESENT)
#define MMC_RSP_R5  (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R6  (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R7  (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)

typedef struct mmc_cmd_struct {
    uint32_t cmd;
    uint32_t arg;
    uint32_t resp;
} mmc_cmd_t;

typedef struct mmc_resp_struct {
    uint32_t resp[4];
    bool is_long_resp;
} mmc_resp_t;

typedef struct mmc_card_struct {

    uint32_t rca;
    uint32_t ocr;
    uint32_t cid[4];
    char product_name[6];
    uint8_t major;
    uint8_t minor;
    void *private_data;
    uint32_t (*host_init)(struct mmc_card_struct *mmc_card);
    uint32_t (*send_command)(struct mmc_card_struct *mmc_card, mmc_cmd_t *cmd);
    uint32_t (*receive_response)(struct mmc_card_struct *mmc_card, mmc_resp_t *resp);
    uint32_t (*read_block)(struct mmc_card_struct *mmc_card, const uint32_t *dst, uint32_t block_id);
    uint32_t (*write_block)(struct mmc_card_struct *mmc_card, const uint32_t *src, uint32_t block_id);
} mmc_card_t;

extern uint32_t mmc_card_init(mmc_card_t *mmc_card);

#define MMC_CARD_SUCCESS                0
#define MMC_CARD_PARAM_NULL             1
#define MMC_HOST_INIT_FAILURE           2
#define MMC_SEND_COMMADN_FAILURE        3
#define MMC_RECEIVE_COMMAND_FAILURE     4
#define MMC_SEND_COMMAND_TIMEOUT        5
#define MMC_SEND_COMMAND_CRC_FAILURE    6
#define MMC_SEND_ACMD_FAILURE           7

#define MMC_DEBUG
#if defined(MMC_DEBUG)
#define MMC_TRACE(fmt, args...)	printf(fmt, ##args)
#else
#define MMC_TRACE(fmt, args...)
#endif

#define MMC_RESPONSE_ACMD_FLAG(card_status)  ((card_status) & (1 << 5))
#define MMC_RESPONSE_BUS_READY(card_status) ((card_status) & (1 << 8))

#endif