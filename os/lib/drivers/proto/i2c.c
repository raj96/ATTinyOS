#include "drivers/proto/i2c.h"
#include "drivers/gpio.h"

// Send I2C_t start signal
void __i2c_send_start(I2C_t *dev) {
    gpio_set_dir(dev->sda, GPIO_OUTPUT);
    gpio_set_dir(dev->scl, GPIO_OUTPUT);

    gpio_write(dev->sda, GPIO_HIGH);
    gpio_write(dev->scl, GPIO_HIGH);
    gpio_write(dev->sda, GPIO_LOW);
}

// Read 1 single bit
inline I2C_RESPONSE __i2c_read_bit(I2C_t *dev) {
    gpio_set_dir(dev->sda, GPIO_INPUT);
    gpio_set_dir(dev->scl, GPIO_INPUT);

    while(gpio_read(dev->scl) == GPIO_LOW);
    I2C_RESPONSE resp = gpio_read(dev->sda);

    gpio_set_dir(dev->sda, GPIO_OUTPUT);
    gpio_set_dir(dev->scl, GPIO_OUTPUT);

    return resp;
}

// Write 1 single bit
inline void __i2c_send_bit(I2C_t *dev, uint8_t bit) {
    gpio_write(dev->scl, GPIO_LOW);
    gpio_write(dev->sda, bit);
    gpio_write(dev->scl, GPIO_HIGH);
    gpio_write(dev->scl, GPIO_LOW);
}

void i2c_init(I2C_t *dev, uint8_t sda, uint8_t scl, uint8_t addr) {
    dev->sda = sda;
    dev->scl = scl;
    dev->addr = addr;

    gpio_set_dir(dev->scl, GPIO_OUTPUT);
    gpio_write(dev->scl, GPIO_HIGH);
}

I2C_RESPONSE i2c_claim_bus(I2C_t *dev, I2C_RW claim_mode) {
    __i2c_send_start(dev);
    i2c_send_byte(dev, dev->addr | claim_mode);

    return __i2c_read_bit(dev);
}

I2C_RESPONSE i2c_send_byte(I2C_t *dev, uint8_t data) {
    dev->addr &= I2C_WRITE;
    __i2c_send_start(dev);

    for(int i = 7; i >= 0; i--) {
        __i2c_send_bit(dev, (data >> i) & 0x1);
    }

    asm volatile("nop");
    asm volatile("nop");

    return __i2c_read_bit(dev);
}

void i2c_release_bus(I2C_t *dev) {
    gpio_write(dev->sda, GPIO_LOW);
    gpio_write(dev->scl, GPIO_HIGH);
    gpio_write(dev->sda, GPIO_HIGH);
}