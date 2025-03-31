#include "drivers/gpio.h"
#include "drivers/timer.h"
#include "drivers/pll.h"

#include <stdint.h>

#define MCUCR *((volatile char *)(0x35 + 0x20))

void kmain() {
    // Blink once at bootup
    gpio_pb1_high();
    for(volatile uint16_t i = 0; i < 65535; i++) {
        asm volatile("nop");
    }
    gpio_pb1_low();
    for(volatile uint16_t i = 0; i < 65535; i++) {
        asm volatile("nop");
    }

    // Check if PLL is enabled
    if(PLLCSR.PLLE == 1) {
        gpio_pb1_high();
    }
    for(volatile uint32_t i = 0; i < 2000000; i++) {
        asm volatile("nop");
    }

    // Blink while PLL is not locked
    for(;;) {
        volatile char x = PLLCSR.PLOCK;
        if(x == 1) {
            break;
        }
        gpio_pb1_high();
        for(volatile uint16_t i = 0; i < 50000; i++);
        gpio_pb1_low();
        for(volatile uint16_t i = 0; i < 50000; i++);
    }
    PLLCSR.PCKE = 1;

    // Turn off LED when PLL is locked
    gpio_pb1_low();

    // asm volatile("sei");

    // Goto sleep
    MCUCR = MCUCR | (1 << 5);
    asm volatile("sleep");
}