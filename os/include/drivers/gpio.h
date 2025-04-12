#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

#define GPIO_NUM 5

#define DDRB    *((volatile uint8_t *)(0x17 + 0x20))
#define PORTB   *((volatile uint8_t *)(0x18 + 0x20))
#define PINB    *((volatile uint8_t *)(0x19 + 0x20))

typedef enum {
    GPIO_INPUT,
    GPIO_OUTPUT
} GPIO_DIR;

typedef enum {
    GPIO_LOW,
    GPIO_HIGH
} GPIO_STATE;

typedef enum {
    PB0,
    PB1,
    PB2,
    PB3,
    PB4,
    // PB5 - not available
} GPIO_PIN;

inline void gpio_write(GPIO_PIN pin, GPIO_STATE state) {
    if(state == GPIO_LOW) {
        DDRB &= ~(1 << pin);
    } else {
        DDRB |= (1 << pin);
    }
}

inline void gpio_set_dir(GPIO_PIN pin, GPIO_DIR dir) {
    if(dir == GPIO_INPUT) {
        PORTB &= ~(1 << pin);
    } else {
        PORTB |= (1 << pin);
    }
}

inline uint8_t gpio_read(GPIO_PIN pin) {
    return PINB & (1 << pin);
}

#endif