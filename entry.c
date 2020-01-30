/*!
 * @author  MJm
 * @date    2020/1/22
 */
#include <imx_uart.h>
#include <imx6ul_iomuxc.h>
#include <imx_gpio.h>
#include <at24cxx.h>
#include <timer.h>
#include <sd_card.h>
#include <imx_usdhc.h>
#include <ff.h>

static void test_led(void);
static void test_button(void);
static void test_at24cxx(void);
static void test_timer(void);
static void test_sdcard(void);
static void test_fatfs(void);

static void delay()
{
    uint32_t i, j = 0;
    for (i = 0; i < 65530; i++)
        for(j = 0; j < 2000; j++);
}

void c_entry()
{
    uart_init();
    timer_init();

    printf("hello imx6ul bare metal:%x\n");


    while(1) {
        //test_led();
        // test_button();
        // test_timer();
        // test_at24cxx();
        // test_sdcard();
        test_fatfs();
    }
}

static void test_led()
{
    uint8_t i = 0;
    imx_gpio_t *gpio1 = (imx_gpio_t *)0x0209c000UL;

    iomuxc_set_mux(IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03, MUX_MODE_ALT5);

    gpio_set_output(gpio1, 3);

    while(1) {
        if ((i % 2) == 0)
            gpio_set_dr(gpio1, 3);
        else
            gpio_clr_dr(gpio1, 3);
        printf("%s:%d\n", __func__, i++);
        delay();
    }
}

static void test_button()
{
    uint32_t button_int_stat = 0;
    uint8_t led_status = 0;
    imx_gpio_t *gpio1 = (imx_gpio_t *)0x0209c000UL;

    iomuxc_set_mux(IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03, MUX_MODE_ALT5);
    gpio_set_output(gpio1, 3);

    iomuxc_set_mux(IOMUXC_SW_MUX_CTL_PAD_UART1_CTS, MUX_MODE_ALT5);
    gpio_set_input(gpio1, 18);
    gpio_set_int_cfg(gpio1, 18, RISING_EDGE);
    gpio_clr_int_stat(gpio1, 18);
    gpio_unmask_int(gpio1, 18);

    while (1) {

        while((button_int_stat = gpio_get_int_stat(gpio1, 18)) == 0) {
            delay();
        }

        printf("button changed\n");
        led_status = ~led_status;
        gpio_clr_int_stat(gpio1, 18);
        if (led_status == 0) {
            gpio_set_dr(gpio1, 3);
        } else {
            gpio_clr_dr(gpio1, 3);
        }
        delay();
    }
}

static void test_at24cxx()
{
    uint8_t i = 0, j = 0;
    uint8_t buf[16] = {0};
    at24cxx_t at24cxx;

    at24cxx_init(&at24cxx);

    for (i = 0; i < 16; i++) {
        at24cxx_write_byte(&at24cxx, i, i);
    }

    for (i = 0; i < 16; i++) {
        j = at24cxx_read_byte(&at24cxx, i);
        printf("%s: j:%d\n", __func__, j);
    }

    at24cxx_read(&at24cxx, 0, buf, 16);
    for (i = 0; i < 16; i++) {
        printf("%s: %d\n", __func__, buf[i]);
    }
    while(1);
}

static void test_timer()
{
    timer_t timer;
    timer_setto(&timer, 5000 * 1000);
    printf("%s: before timerout\n", __func__);
    while (timer_isto(&timer) == false);
    printf("%s: after timerout\n", __func__);
    while(1);
}

static void test_sdcard()
{
    sdcard_t sdcard;
    imx_usdhc_t *usdhc = (imx_usdhc_t *)0x02190000;
    uint8_t buf[512];
    uint8_t buf2[512];
    uint32_t i;
    sdcard.host = usdhc;
    sdcard.rca = 0x45670000;
    sdcard.host_init = usdhc_init;
    sdcard.send_cmd = usdhc_send_command;
    sdcard.get_resp = usdhc_get_response;
    sdcard.read_block = usdhc_read_block;
    sdcard.write_block = usdhc_write_block;

    printf("\ninit buf as 0\n");
    for (i = 0; i < 16; i++) {
        printf("%x ", buf[i]);
    }
    printf("\n");

    sdcard_init(&sdcard);
    sdcard_read_block(&sdcard, buf, 0x200);
    for (i = 0; i < 16; i++) {
        printf("%x ", buf[i]);
    }
    printf("\n");
    printf("\nread sdcard before write\n");
    for (i = 0; i < 16; i++) {
        printf("%x ", buf[i]);
        buf2[i] = buf[i] + 1;
    }
    printf("\n");
    printf("write sdcard by add 1\n");
    sdcard_write_block(&sdcard, buf2, 0);
    sdcard_read_block(&sdcard, buf, 0);
    printf("\nread sdcard after write\n");
    for (i = 0; i < 16; i++) {
        printf("%x ", buf[i]);
    }
    // printf("\n");
        
    while(1);
}

static void dump_fatfs(FATFS *fatfs_p)
{
    printf("===========%s=========\n", __func__);
    printf("    fs_type: %x        \n", fatfs_p->fs_type);
    printf("    pdrv: %x           \n", fatfs_p->pdrv);
    printf("    csize: %x          \n", fatfs_p->csize);
    printf("    n_fats: %x         \n", fatfs_p->n_fats);
    printf("    wflag: %x          \n", fatfs_p->wflag);
    printf("    fsi_flag: %x       \n", fatfs_p->fsi_flag);
    printf("    id: %x             \n", fatfs_p->id);
    printf("    n_rootdir: %x      \n", fatfs_p->n_rootdir);
    printf("    last_clst:%x       \n", fatfs_p->last_clst);
    printf("    free_clst:%x       \n", fatfs_p->free_clst);
    printf("    n_fatent:%x        \n", fatfs_p->n_fatent);
    printf("    fsize:%x           \n", fatfs_p->fsize);
    printf("    volbase:%x         \n", fatfs_p->volbase);
    printf("    fatbase:%x         \n", fatfs_p->fatbase);
    printf("    dirbase:%x         \n", fatfs_p->dirbase);
    printf("    winsect:%x         \n", fatfs_p->winsect);
    printf("======================\n");
}


static void test_fatfs()
{
    FATFS fs;
    FIL file;
    char buf[64];
    char *test_str = "test fatfs string";
    uint32_t len = 0;
    FRESULT res = 0;

    printf("%s entry\n", __func__);

    res = f_mount(&fs,"0:",1); 
    printf("%s f_mount res:%d\n", __func__, res);
    dump_fatfs(&fs);

    res = f_open(&file, "aa.txt", FA_READ);
    printf("%s f_open res:%d\n", __func__, res);
    f_read(&file, buf, 64, &len);
    printf("%s read content: buf:%s\n", __func__, buf);
    f_close(&file);

    res = f_open(&file, "bb.txt", FA_CREATE_NEW | FA_WRITE);
    f_write(&file, test_str, 64, &len);
    f_close(&file);

    res = f_open(&file, "bb.txt", FA_READ);
    printf("%s f_open res:%d\n", __func__, res);
    f_read(&file, buf, 64, &len);
    printf("%s read content: buf:%s\n", __func__, buf);
    f_close(&file);
    
    while(1);
}