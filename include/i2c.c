/**
 * @file    i2c.c
 * @brief   Bit-banged software I2C driver for the CH55x (8051) MCU.
 * @author  Zhiyuan Wan
 * @license MIT
 * @version V1.0
 * @date    2018/03/17
 */
#include "i2c.h"
#include "ch55x.h"
#include <stdint.h>

SBIT(I2C_SDAT, 0xB0, 4); // P3.4
SBIT(I2C_SCLK, 0xB0, 3); // P3.3

/**
 * @brief   Initialise the GPIO pins used for the I2C bus (idle = both lines high).
 */
void i2c_init() { // Initialize GPIO pins for I2C
    I2C_SDAT = 1;
    I2C_SCLK = 1;
}

/**
 * @brief   Short delay used to shape I2C timing.
 */
void i2c_delay() {
    volatile char i = 1;
    while (i--)
        ;
}

/**
 * @brief   Generate an I2C START condition (high-to-low transition on SDA
 *          while SCL is high).
 */
void i2c_start() {
    I2C_SDAT = 1;
    I2C_SCLK = 1;
    i2c_delay();

    I2C_SDAT = 0;
    i2c_delay();

    I2C_SCLK = 0;
    i2c_delay();
}

/**
 * @brief   Generate an I2C STOP condition (low-to-high transition on SDA
 *          while SCL is high).
 */
void i2c_stop() {
    I2C_SDAT = 0;
    I2C_SCLK = 1;
    i2c_delay();

    I2C_SDAT = 1;
    i2c_delay();
}

/**
 * @brief   Transmit one byte (MSB first) on the I2C bus.
 * @param   data  Byte to transmit.
 */
void i2c_write(unsigned char data) {
    int i;

    for (i = 0; i < 8; i++) {
        data <<= 1;
        I2C_SDAT = CY;

        I2C_SCLK = 1;
        i2c_delay();

        I2C_SCLK = 0;
        i2c_delay();
    }
}

/**
 * @brief   Read one byte (MSB first) from the I2C bus.
 * @return  Received byte.
 */
unsigned char i2c_read() {
    int i;
    uint8_t ret = 0;

    I2C_SDAT = 1;
    for (i = 0; i < 8; i++) {
        ret <<= 1;
        I2C_SCLK = 1;
        i2c_delay();

        if (I2C_SDAT)
            ret |= 0x01;

        I2C_SCLK = 0;
        i2c_delay();
    }
    return ret;
}

/**
 * @brief   Generate a 9th clock and sample the ACK/NAK bit from the slave.
 * @return  true if the slave pulled SDA low (ACK), false otherwise (NAK).
 */
bool i2c_read_ack() {
    bool status;

    I2C_SDAT = 1;

    I2C_SCLK = 1;
    i2c_delay();

    status = I2C_SDAT;

    I2C_SCLK = 0;
    i2c_delay();

    return !status;
}

/**
 * @brief   Convenience wrapper: report whether the slave signalled NAK.
 * @return  true if the slave released SDA (NAK), false on ACK.
 */
bool i2c_read_nak() { return !i2c_read_ack(); }
