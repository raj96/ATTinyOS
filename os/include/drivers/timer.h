#ifndef TIMER
#define TIMER

typedef struct {
    char WGM00_01:2;
    char rsvd:2;
    char COM0B0:1;
    char COM0B1:1;
    char COM0A0:1;
    char COM0A1:1;
} TCCR0A_t;

#define TCCR0A  (*(volatile TCCR0A_t *)(0x2A + 0x20))

typedef struct {
    char CS:3;
    char WGM02:1;
    char rsvd:2;
    char FOC0B:1;
    char FOC0A:1;
} TCCR0B_t;

#define TCCR0B  (*(volatile TCCR0B_t *)(0x33 + 0x20))
#define OCR0A   (*(volatile char *)(0x29 + 0x20))
#define OCR0B   (*(volatile char *)(0x28 + 0x20))

typedef struct {
    char rsvd:1;
    char TOIE0:1;
    char TOIE1:1;
    char OCIE0B:1;
    char OCIE0A:1;
    char OCIE1B:1;
    char OCIE1A:1;
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