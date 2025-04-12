#include "drivers/i2c.h"
#include "drivers/gpio.h"

// Send I2C start signal
void __i2c_send_start(I2C *dev) {
    gpio_set_dir(dev->sda, GPIO_OUTPUT);
    gpio_set_dir(dev->scl, GPIO_OUTPUT);

    gpio_write(dev->sda, GPIO_LOW);
    gpio_write(dev->scl, GPIO_LOW);
}

// Send I2C stop signal
void __i2c_send_stop(I2C *dev) {
    gpio_write(dev->scl, GPIO_HIGH);
    gpio_write(dev->sda, GPIO_HIGH);
}

// Read 1 single bit
inline I2C_RESPONSE __i2c_read_bit(I2C *dev) {
    gpio_set_dir(dev->sda, GPIO_INPUT);

    gpio_write(dev->scl, GPIO_LOW);
    gpio_write(dev->scl, GPIO_HIGH);

    I2C_RESPONSE resp = gpio_read(dev->sda);

    gpio_write(dev->scl, GPIO_LOW);

    gpio_set_dir(dev->sda, GPIO_OUTPUT);

    return resp;
}

// Write 1 single bit
inline void __i2c_send_bit(I2C *dev, uint8_t bit) {
    gpio_write(dev->scl, GPIO_LOW);
    gpio_write(dev->sda, bit);
    gpio_write(dev->scl, GPIO_HIGH);
    gpio_write(dev->scl, GPIO_LOW);
}

void i2c_init(I2C *dev, uint8_t sda, uint8_t scl, uint8_t addr) {
    dev->sda = sda;
    dev->scl = scl;
    dev->addr = addr;
}

I2C_RESPONSE i2c_send_byte(I2C *dev, uint8_t data) {
    dev->addr &= I2C_WRITE;
    __i2c_send_start(dev);

    for(int i = 7; i >= 0; i--) {
        __i2c_send_bit(dev, (data >> i) & 0x1);
    }

    return __i2c_read_bit(dev);
}