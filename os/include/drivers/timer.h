#ifndef TIMER
#define TIMER

#include <stdint.h>

typedef struct {
    uint8_t WGM00_01:2;
    uint8_t rsvd:2;
    uint8_t COM0B0:1;
    uint8_t COM0B1:1;
    uint8_t COM0A0:1;
    uint8_t COM0A1:1;
} TCCR0A_t;

#define TCCR0A  (*(volatile TCCR0A_t *)(0x2A + 0x20))

typedef struct {
    uint8_t CS:3;
    uint8_t WGM02:1;
    uint8_t rsvd:2;
    uint8_t FOC0B:1;
    uint8_t FOC0A:1;
} TCCR0B_t;

#define TCCR0B  (*(volatile TCCR0B_t *)(0x33 + 0x20))
#define OCR0A   (*(volatile uint8_t *)(0x29 + 0x20))
#define OCR0B   (*(volatile uint8_t *)(0x28 + 0x20))

typedef struct {
    uint8_t rsvd:1;
    uint8_t TOIE0:1;
    uint8_t TOIE1:1;
    uint8_t OCIE0B:1;
    uint8_t OCIE0A:1;
    uint8_t OCIE1B:1;
    uint8_t OCIE1A:1;
} TIMSK_t;

#define TIMSK   (*(volatile TIMSK_t *)(0x39 + 0x20))

inline void timer0_enable_overflow_interrupt() {
    TIMSK.TOIE0 = 1;
}

inline void timer0_disable_overflow_interrupt() {
    TIMSK.TOIE0 = 0;
}

typedef enum {
    CLK_STOP = 0,
    CLK_RAW,
    CLK_DIV_8,
    CLK_DIV_64,
    CLK_DIV_256,
    CLK_DIV_1024,
    CLK_EXT_FALL,
    CLK_EXT_RISE
} Timer0Prescalers;

inline void timer0_set_prescaler(Timer0Prescalers prescaler) {
    TCCR0B.CS = prescaler;
}

typedef enum {
    WGM_NORMAL = 0,
    WGM_PWM_0xFF,
    WGM_CTC,
    WGM_FAST_PWM_0xFF,
    X_WGM_RSVD,
    WGM_PWM_OCRA,
    X_WGM_RSVD_1,
    WGM_FAST_PWM_OCRA
} Timer0WGModes;

inline void timer0_set_wgm(Timer0WGModes wgm_mode) {
    TCCR0A.WGM00_01 = wgm_mode & 3;
    TCCR0B.WGM02 = wgm_mode >> 2;
}

#endif