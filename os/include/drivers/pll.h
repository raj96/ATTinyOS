#ifndef PLL_H
#define PLL_H

#include <stdint.h>

typedef struct {
    uint8_t PLOCK:1;
    uint8_t PLLE:1;
    uint8_t PCKE:1;
    uint8_t rsvd:4;
    uint8_t lsm:1;
} PLLCSR_t;

#define PLLCSR (*(volatile PLLCSR_t *)(0x27 + 0x20))

#endif