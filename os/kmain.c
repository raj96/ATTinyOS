#include "drivers/gpio.h"
#include "drivers/timer.h"
#include "drivers/pll.h"
#include "scheduler/tasker.h"
#include "drivers/device/ssd1306.h"

#include <stdint.h>

inline void nop() {
    asm volatile("nop");
}

void kmain() {
    gpio_set_dir(PB0, GPIO_OUTPUT);
    gpio_set_dir(PB1, GPIO_OUTPUT);
    // // Check if PLL is enabled
    // if(PLLCSR.PLLE != 1) {
    //     PLLCSR.PLLE = 1;
    //     gpio_write(PB1, GPIO_HIGH);
    //     for(volatile uint32_t i = 0; i < 200000; i++) {
    //         asm volatile("nop");
    //     }
    // }
    
    // Blink while PLL is not locked
    for(;;) {
        if(PLLCSR.PLOCK == 1) {
            break;
        }
        
        gpio_write(PB1, GPIO_HIGH);
        for(volatile uint16_t i = 0; i < 50000; i++);
        gpio_write(PB1, GPIO_LOW);
        for(volatile uint16_t i = 0; i < 50000; i++);
        
    }
    PLLCSR.PCKE = 1;

    for(volatile uint8_t i = 0 ; i < 255; i++);

    nop();

    I2C_t ssd1306_i2c;
    i2c_init(&ssd1306_i2c, 0, 1, 0x78);

    SSD1306_d ssd1306_dev;
    ssd1306_init(&ssd1306_dev, &ssd1306_i2c, true);
    ssd1306_set_addr_mode(&ssd1306_dev, SSD1306_PM_H);
    ssd1306_clrscr(&ssd1306_dev);
    
    // for(int i = 0; i < 10; i++) {
    //     ssd1306_point(&ssd1306_dev, i, i);
    // }

    int8_t delta = 1;
    uint8_t minY = 0;
    uint8_t maxY = 63;
    int8_t y = minY;
    for (uint8_t x = 0; x < 128; x++) {
        ssd1306_point(&ssd1306_dev, x, y);
        ssd1306_point(&ssd1306_dev, x+1, y);
        y += delta;
        if(y > maxY) {
            delta = -1;
            y = maxY - 1;
        } else if (y < minY) {
            delta = 1;
            y = minY + 1;
        }
    }

    i2c_release_bus(&ssd1306_i2c);

    // Goto sleep
    while(1) {
        asm volatile("nop");
    }
}