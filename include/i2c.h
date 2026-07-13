/**
 * @file    i2c.h
 * @brief   Bit-banged software I2C driver for the CH55x (8051) MCU.
 * @author  Zhiyuan Wan
 * @license MIT
 * @version V1.0
 * @date    2018/03/17
 */
#ifndef _I2C_H_

#define _I2C_H_

typedef __bit bool;

/**
 * @brief   Initialise the GPIO pins used for the I2C bus (idle = both lines high).
 */
extern void i2c_init();

/**
 * @brief   Generate an I2C START condition (high-to-low transition on SDA
 *          while SCL is high).
 */
extern void i2c_start();

/**
 * @brief   Generate an I2C STOP condition (low-to-high transition on SDA
 *          while SCL is high).
 */
extern void i2c_stop();

/**
 * @brief   Transmit one byte (MSB first) on the I2C bus.
 * @param   data  Byte to transmit.
 */
extern void i2c_write(unsigned char data);

/**
 * @brief   Generate a 9th clock and sample the ACK/NAK bit from the slave.
 * @return  true if the slave pulled SDA low (ACK), false otherwise (NAK).
 */
extern bool i2c_read_ack();

/**
 * @brief   Convenience wrapper: report whether the slave signalled NAK.
 * @return  true if the slave released SDA (NAK), false on ACK.
 */
extern bool i2c_read_nak();

/**
 * @brief   Read one byte (MSB first) from the I2C bus.
 * @return  Received byte.
 */
extern unsigned char i2c_read();

#define TW_READ 0x01

#endif
