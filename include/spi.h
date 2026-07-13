/**
 * @file    spi.h
 * @brief   CH55x SPI master/slave mode interface.
 * @author  WCH
 * @version V1.0
 * @date    2017/07/05
 */
#pragma once

#include <stdint.h>

/** @brief Set the SPI clock divider. */
#define SPI_CK_SET(n) (SPI0_CK_SE = n)

/** @brief Assert the SPI master chip-select (active low). */
#define SPIMasterAssertCS() (SCS = 0)
/** @brief Deassert the SPI master chip-select. */
#define SPIMasterDeassertCS() (SCS = 1)

/**
 * @brief   Initialise the SPI in master mode.
 * @param   mode  SPI mode (0 or 3).
 */
void SPIMasterModeSet(uint8_t mode);

/**
 * @brief   Enable SPI0 interrupts (one-byte receive + FIFO overflow).
 */
void CH554SPIInterruptInit();

/**
 * @brief   Write one byte to the SPI bus in master mode.
 * @param   dat  Byte to transmit.
 */
void CH554SPIMasterWrite(uint8_t dat);

/**
 * @brief   Read one byte from the SPI0 bus in master mode.
 * @return  Received byte.
 */
uint8_t CH554SPIMasterRead();

/**
 * @brief   Initialise the SPI in slave mode.
 */
void SPISlvModeSet();

/**
 * @brief   Write one byte to the SPI0 bus in slave mode.
 * @param   dat  Byte to transmit.
 */
void CH554SPISlvWrite(uint8_t dat);

/**
 * @brief   Read one byte from the SPI0 bus in slave mode.
 * @return  Received byte.
 */
uint8_t CH554SPISlvRead();
