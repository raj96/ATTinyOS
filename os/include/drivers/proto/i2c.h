#ifndef I2C_H
#define I2C_H

#include <stdint.h>

typedef enum {
    I2C_ACK,
    I2C_NACK
} I2C_RESPONSE;

typedef enum {
    I2C_WRITE,
    I2C_READ
} I2C_RW;

typedef struct {
    uint8_t sda:3;
    uint8_t scl:3;
    uint8_t claimed:1;

    uint8_t addr;
} I2C_t;

// Initialize I2C device as master
void i2c_init(I2C_t *i2c_dev, uint8_t sda, uint8_t scl, uint8_t addr);

// Claim i2c bus in r/w mode
I2C_RESPONSE i2c_claim_bus(I2C_t *i2c_dev, I2C_RW claim_mode);

// Send a byte over SDA line
I2C_RESPONSE i2c_send_byte(I2C_t *i2c_dev, uint8_t data);

// Release i2c bus
void i2c_release_bus(I2C_t *i2c_dev);

#endif