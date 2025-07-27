#ifndef SSD1306_H
#define SSD1306_H

#include "drivers/proto/i2c.h"
#include <stdbool.h>

#define CONTROL_BYTE(co, dc)  ((co << 1 | dc) << 6)

#define SSD1306_CMD_SET_COLUMN      0x21
#define SSD1306_CMD_SET_PAGE        0x22
#define SSD1306_CMD_SET_SCROLL_HR   0x26
#define SSD1306_CMD_SET_SCROLL_HL   0x27
#define SSD1306_CMD_SET_CONTRAST    0x81
#define SSD1306_CMD_STOP_SCROLL     0x2e
#define SSD1306_CMD_START_SCROLL    0x2f
#define SSD1306_CMD_NOP             0xe3

typedef enum {
    SSD1306_PM_H = 0,
    SSD1306_PM_V,
    SSD1306_PM_P,
} SSD1306_PageMode;

typedef enum {
    SSD1306_SM_HR = 0,
    SSD1306_SM_HL = 1,
} SSD1306_ScrollMode;

typedef enum {
    SSD1306_SI_5 = 0,
    SSD1306_SI_64,
    SSD1306_SI_128,
    SSD1306_SI_256,
    SSD1306_SI_3,
    SSD1306_SI_4,
    SSD1306_SI_25,
    SSD1306_SI_2,
} SSD1306_ScrollInterval;

typedef struct {
    I2C_t *i2c;
    SSD1306_PageMode page_mode;
    uint8_t current_page;
} SSD1306_d;

void ssd1306_init(SSD1306_d *dev, I2C_t *idev, bool charge_pump);
void ssd1306_set_contrast(SSD1306_d *dev, uint8_t contrast);
void ssd1306_set_addr_mode(SSD1306_d *dev, SSD1306_PageMode mode);
void ssd1306_clrscr(SSD1306_d *dev);
void ssd1306_point(SSD1306_d *dev, uint8_t x, uint8_t y);
void ssd1306_set_scroll(SSD1306_d *dev, SSD1306_ScrollMode mode, uint8_t start_page, SSD1306_ScrollInterval interval, uint8_t end_page);
void ssd1306_start_scroll(SSD1306_d *dev);
void ssd1306_stop_scroll(SSD1306_d *dev);

#endif