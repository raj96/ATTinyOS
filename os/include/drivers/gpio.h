#ifndef GPIO
#define GPIO

inline void gpio_pb0_low() {
    asm volatile("sbi 0x17, 0");
    asm volatile("cbi 0x18, 0");
}

inline void gpio_pb0_high() {
    asm volatile("sbi 0x17, 0");
    asm volatile("sbi 0x18, 0");
}

inline void gpio_pb1_low() {
    asm volatile("sbi 0x17, 1");
    asm volatile("cbi 0x18, 1");
}

inline void gpio_pb1_high() {
    asm volatile("sbi 0x17, 1");
    asm volatile("sbi 0x18, 1");
}

inline void gpio_pb2_low() {
    asm volatile("sbi 0x17, 2");
    asm volatile("cbi 0x18, 2");
}

inline void gpio_pb2_high() {
    asm volatile("sbi 0x17, 2");
    asm volatile("sbi 0x18, 2");
}

inline void gpio_pb3_low() {
    asm volatile("sbi 0x17, 3");
    asm volatile("cbi 0x18, 3");
}

inline void gpio_pb3_high() {
    asm volatile("sbi 0x17, 3");
    asm volatile("sbi 0x18, 3");
}

inline void gpio_pb4_low() {
    asm volatile("sbi 0x17, 4");
    asm volatile("cbi 0x18, 4");
}

inline void gpio_pb4_high() {
    asm volatile("sbi 0x17, 4");
    asm volatile("sbi 0x18, 4");
}

inline void gpio_pb5_low() {
    asm volatile("sbi 0x17, 4");
    asm volatile("cbi 0x18, 4");
}

inline void gpio_pb5_high() {
    asm volatile("sbi 0x17, 4");
    asm volatile("sbi 0x18, 4");
}

#endif