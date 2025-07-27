#include "drivers/device/ssd1306.h"
#include "drivers/gpio.h"

inline void __ssd1306_start_read(SSD1306_d *dev) {
    while(1) {
        if (i2c_claim_bus(dev->i2c, I2C_READ) == I2C_ACK) break;
    }
}

inline void __ssd1306_start_write(SSD1306_d *dev) {
    while(1) {
        if (i2c_claim_bus(dev->i2c, I2C_WRITE) == I2C_ACK) break;
    }
}

inline void __ssd1306_stop(SSD1306_d *dev) {
    i2c_release_bus(dev->i2c);
}

void ssd1306_init(SSD1306_d *dev, I2C_t *idev, bool charge_pump) {
    dev->i2c = idev;
    dev->page_mode = SSD1306_PM_H;

    __ssd1306_start_write(dev);

    for(volatile uint16_t i = 0; i < 15000; i++);
    if(charge_pump) {
        i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 0));
        i2c_send_byte(dev->i2c, 0x8d);
        i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 0));
        i2c_send_byte(dev->i2c, 0x14);
        for(volatile uint16_t i = 0; i < 15000; i++);
    }
    
    // Enable display
    i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 0));
    i2c_send_byte(dev->i2c, 0xaf);

    // Invert display
    i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 0));
    i2c_send_byte(dev->i2c, 0xa6);

    // __ssd1306_stop(dev);
}

void ssd1306_set_addr_mode(SSD1306_d *dev, SSD1306_PageMode mode) {
    // __ssd1306_start_write(dev);
    i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 0));
    i2c_send_byte(dev->i2c, 0x20);

    i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 0));
    i2c_send_byte(dev->i2c, mode);
    // __ssd1306_stop(dev);

    dev->page_mode = mode;
}

void ssd1306_set_contrast(SSD1306_d *dev, uint8_t contrast) {
    // __ssd1306_start_write(dev);
    i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 0));
    i2c_send_byte(dev->i2c, SSD1306_CMD_SET_CONTRAST);
    i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 0));
    i2c_send_byte(dev->i2c, contrast);
    // __ssd1306_stop(dev);
}

void ssd1306_clrscr(SSD1306_d *dev) {
    dev->current_page = 0;

    // __ssd1306_start_write(dev);

    switch(dev->page_mode) {
        case SSD1306_PM_H:
        case SSD1306_PM_V:
            // Reset Column range
            i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 0));
            i2c_send_byte(dev->i2c, SSD1306_CMD_SET_COLUMN);
            i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 0));
            i2c_send_byte(dev->i2c, 0);
            i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 0));
            i2c_send_byte(dev->i2c, 127);
            // Reset Page range
            i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 0));
            i2c_send_byte(dev->i2c, SSD1306_CMD_SET_PAGE);
            i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 0));
            i2c_send_byte(dev->i2c, 0);
            i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 0));
            i2c_send_byte(dev->i2c, 7);

            for(uint8_t y = 0; y < 128; y++) {
                for(uint8_t x = 0; x < 8; x++) {
                    i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 1));
                    i2c_send_byte(dev->i2c, 0);
                }
            }
            break;
        case SSD1306_PM_P:
            break;
    }

    // __ssd1306_stop(dev);
}

int mod(int x, int y) {
    while(x >= y) {
        x -= y;
    }

    return x;
}

int div(int x, int y) {
    int rem = 0;
    while(x >= y) {
        x -= y;
        rem++;
    }

    return rem;
}

void ssd1306_point(SSD1306_d *dev, uint8_t x, uint8_t y) {
    // __ssd1306_start_write(dev);

    i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 0));
    i2c_send_byte(dev->i2c, SSD1306_CMD_SET_COLUMN);
    i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 0));
    i2c_send_byte(dev->i2c, x);
    i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 0));
    i2c_send_byte(dev->i2c, x);

    uint8_t page = div(y, 8);
    i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 0));
    i2c_send_byte(dev->i2c, SSD1306_CMD_SET_PAGE);
    i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 0));
    i2c_send_byte(dev->i2c, page);
    i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 0));
    i2c_send_byte(dev->i2c, page);

    uint8_t bit = mod(y, 8);
    i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 1));
    i2c_send_byte(dev->i2c, 1 << bit);

    i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 0));
    i2c_send_byte(dev->i2c, SSD1306_CMD_NOP);

    // __ssd1306_stop(dev);
}

void ssd1306_set_scroll(SSD1306_d *dev, SSD1306_ScrollMode mode, uint8_t start_page, SSD1306_ScrollInterval interval, uint8_t end_page) {
    // __ssd1306_start_write(dev);

    i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 0));

    switch(mode) {
        case SSD1306_SM_HL:
            i2c_send_byte(dev->i2c, SSD1306_CMD_SET_SCROLL_HL);
            break;
        case SSD1306_SM_HR:
            i2c_send_byte(dev->i2c, SSD1306_CMD_SET_SCROLL_HR);
            break;
    }

    i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 0));
    i2c_send_byte(dev->i2c, 0x00);

    i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 0));
    i2c_send_byte(dev->i2c, start_page);

    i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 0));
    i2c_send_byte(dev->i2c, (uint8_t)interval);

    i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 0));
    i2c_send_byte(dev->i2c, end_page);

    i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 0));
    i2c_send_byte(dev->i2c, 0x00);

    i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 0));
    i2c_send_byte(dev->i2c, 0xff);

    // __ssd1306_stop(dev);
}

void ssd1306_start_scroll(SSD1306_d *dev) {
    // __ssd1306_start_write(dev);
    i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 0));
    i2c_send_byte(dev->i2c, SSD1306_CMD_START_SCROLL);
    // __ssd1306_stop(dev);
}

void ssd1306_stop_scroll(SSD1306_d *dev) {
    // __ssd1306_start_write(dev);
    i2c_send_byte(dev->i2c, CONTROL_BYTE(1, 0));
    i2c_send_byte(dev->i2c, SSD1306_CMD_STOP_SCROLL);
    // __ssd1306_stop(dev);
}
