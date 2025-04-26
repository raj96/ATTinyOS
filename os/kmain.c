#include "drivers/gpio.h"
#include "drivers/timer.h"
#include "drivers/pll.h"
#include "scheduler/tasker.h"

#include <stdint.h>

// extern void load_sp_and_pc();
// extern void c_store();
// extern void c_load();

// extern Task_t *current_task;

inline void delay_arb() {
    asm volatile("ldi YL, 255");
    asm volatile("ldi ZH, 255");
    asm volatile("ldi ZL, 255");
    asm volatile("sbiw ZL, 1");
    asm volatile("brne .-4");
    asm volatile("dec YL");
    asm volatile("brne .-12");
}

#define DELAY 5000

void blink_pb1() {
    gpio_set_dir(PB1, GPIO_OUTPUT);

    while(1) {
        gpio_write(PB1, GPIO_LOW);
        for(volatile uint32_t i = 0; i < 6 * DELAY; i++);
        // for(volatile uint32_t i = 0; i < 4 * DELAY; i++);
        // delay_arb();
        gpio_write(PB1, GPIO_HIGH);
        // delay_arb();
        for(volatile uint32_t i = 0; i <  DELAY/5; i++);
    }
}

void blink_pb0() {
    gpio_set_dir(PB0, GPIO_OUTPUT);

    while(1) {
        gpio_write(PB0, GPIO_LOW);
        // delay_arb();
        for(volatile uint32_t i = 0; i < DELAY/5; i++);
        gpio_write(PB0, GPIO_HIGH);
        // delay_arb();
        for(volatile uint32_t i = 0; i < DELAY/5; i++);
    }
}

void nop(volatile void *a) {
    asm volatile("nop");
}

void kmain() {
    // gpio_set_dir(PB1, GPIO_OUTPUT);
    // // Check if PLL is enabled
    // if(PLLCSR.PLLE != 1) {
    //     PLLCSR.PLLE = 1;
    //     gpio_write(PB1, GPIO_HIGH);
    //     for(volatile uint32_t i = 0; i < 200000; i++) {
    //         asm volatile("nop");
    //     }
    // }
    
    // // Blink while PLL is not locked
    // for(;;) {
    //     if(PLLCSR.PLOCK == 1) {
    //         break;
    //     }
        
    //     gpio_write(PB1, GPIO_HIGH);
    //     for(volatile uint16_t i = 0; i < 50000; i++);
    //     gpio_write(PB1, GPIO_LOW);
    //     for(volatile uint16_t i = 0; i < 50000; i++);
        
    // }
    // PLLCSR.PCKE = 1;

    for(volatile uint8_t i = 0 ; i < 255; i++);

    asm volatile("nop");

    tasker_init();

    // tasks[0].queued = 1;
    // tasks[0].pch = ((uint16_t)(&blink_pb1) >> 8) & 0x0f;
    // tasks[0].pcl = ((uint16_t)(&blink_pb1)) & 0xff;
    // tasks[0].sph = 0x0f;
    // tasks[0].spl = 0xff;
    // tasks[0].sreg = 0;

    // tasks[1].queued = 1;
    // tasks[1].pch = ((uint16_t)(&blink_pb0) >> 8) & 0x0f;
    // tasks[1].pcl = ((uint16_t)(&blink_pb0)) & 0xff;
    // tasks[1].sph = 0x00;
    // tasks[1].spl = 0xff;
    // tasks[1].sreg = 0;

    // current_task = &tasks[0];

    // load_sp_and_pc();

    tasker_add(&blink_pb0);
    tasker_add(&blink_pb1);
    
    tasker_run();

    // Goto sleep
    while(1) {
        asm volatile("nop");
    }
}