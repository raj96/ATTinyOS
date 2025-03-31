#ifndef PLL_H
#define PLL_H

typedef struct {
    volatile char PLOCK:1;
    volatile char PLLE:1;
    volatile char PCKE:1;
    char rsvd:4;
    volatile char lsm:1;
} PLLCSR_t;

#define PLLCSR (*(volatile PLLCSR_t *)(0x27 + 0x20))

#endif