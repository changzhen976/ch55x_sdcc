/**
 * @file    spi.c
 * @brief   CH55x SPI master/slave mode interface.
 * @author  WCH
 * @version V1.0
 * @date    2017/07/05
 *
 * @note    While chip-select (SCS) is asserted, the slave automatically loads the
 *          preset value of SPI0_S_PRE into its transmit shift buffer. It is best
 *          to write the first byte to send into SPI0_S_PRE before SCS goes active,
 *          or have the master discard the first received byte. Note that the
 *          master will first consume the SPI0_S_PRE value and raise an
 *          S0_IF_BYTE interrupt. If the slave must transmit first right after SCS
 *          is asserted, put the first output byte in SPI0_S_PRE; once SCS is
 *          already active, use SPI0_DATA for subsequent bytes.
 */

#include "spi.h"
#include <ch55x.h>

/**
 * @brief   Initialise the SPI in master mode.
 * @param   mode  SPI mode (0 or 3).
 */
void SPIMasterModeSet(uint8_t mode) {
    SPI0_SETUP = 0; // Master mode, MSB first
    if (mode == 0) {
        SPI0_CTRL = 0x60; // Mode 0
    } else if (mode == 3) {
        SPI0_CTRL = 0x68; // Mode 3
    }
    P1_MOD_OC &= 0x0F;
    P1_DIR_PU |= 0xB0; // SCS, MOSI, SCK as push-pull output
    P1_DIR_PU &= 0xBF; // MISO disable pull-up

    // Set clock speed
    SPI0_CK_SE = 0x02;
}

/**
 * @brief   Enable SPI0 interrupts (one-byte receive + FIFO overflow).
 */
void CH554SPIInterruptInit() {
    //IP_EX |= bIP_SPI0; // SPI0 interrupt priority configuration
    SPI0_SETUP |=
        bS0_IE_FIFO_OV | bS0_IE_BYTE; // Enable 1-byte receive interrupt and FIFO overflow interrupt
    SPI0_CTRL |= bS0_AUTO_IF; // Auto-clear S0_IF_BYTE interrupt flag
    SPI0_STAT |= 0xff;        // Clear SPI0 interrupt flags
#ifdef SPI_Interrupt
    IE_SPI0 = 1; // Enable SPI0 interrupt
#endif
}

/**
 * @brief   Write one byte to the SPI bus in master mode.
 * @param   dat  Byte to transmit.
 */
void CH554SPIMasterWrite(uint8_t dat) {
    SPI0_DATA = dat;
    while (S0_FREE == 0)
        ; // Wait for transfer to complete
    // If bS0_DATA_DIR is 1, one byte can be read back here for fast read/write
}

/**
 * @brief   Read one byte from the SPI0 bus in master mode.
 * @return  Received byte.
 */
uint8_t CH554SPIMasterRead() {
    SPI0_DATA = 0xff;
    while (S0_FREE == 0)
        ;
    return SPI0_DATA;
}

/**
 * @brief   Initialise the SPI in slave mode.
 */
void SPISlvModeSet() {
    SPI0_SETUP = 0x80; // Slave mode, MSB first
    SPI0_CTRL = 0x81;  // Read/write FIFO, auto-clear S0_IF_BYTE flag
    P1_MOD_OC &= 0x0F;
    P1_DIR_PU &= 0x0F; // SCS, MOSI, SCK, MISO all floating input
}

/**
 * @brief   Write one byte to the SPI0 bus in slave mode.
 * @param   dat  Byte to transmit.
 */
void CH554SPISlvWrite(uint8_t dat) {
    SPI0_DATA = dat;
    while (S0_IF_BYTE == 0)
        ;
    S0_IF_BYTE = 0;
}

/**
 * @brief   Read one byte from the SPI0 bus in slave mode.
 * @return  Received byte.
 */
uint8_t CH554SPISlvRead() {
    while (S0_IF_BYTE == 0)
        ;
    S0_IF_BYTE = 0;
    return SPI0_DATA;
}
