/**
 * @file    debug.h
 * @brief   CH55x debug interface: main-frequency configuration, delay routines,
 *          UART0/UART1 setup and putchar/getchar for printf.
 * @author  WCH
 * @version V1.0
 * @date    2017/01/20
 */
#pragma once

#include "ch55x.h"
#include <stdint.h>

#ifndef UART0_BAUD
#define UART0_BAUD 9600
#endif

#ifndef UART1_BAUD
#define UART1_BAUD 9600
#endif

/**
 * @brief   Select and configure the CH55x system clock (Fsys).
 *
 * Fsys defaults to 6 MHz; it is derived from FREQ_SYS via CLOCK_CFG:
 *   Fsys = (Fosc * 4) / (CLOCK_CFG & MASK_SYS_CK_SEL)
 */
void CfgFsys(void);

/**
 * @brief   Microsecond delay.
 * @param   n  Number of microseconds to wait (calibrated to ~1us @ Fsys=12MHz).
 */
void mDelayuS(uint16_t n);

/**
 * @brief   Millisecond delay.
 * @param   n  Number of milliseconds to wait.
 */
void mDelaymS(uint16_t n);

/**
 * @brief   Remap UART0 to alternate pins P1.2 (TX) and P1.3 (RX).
 */
inline void CH554UART0Alter(void) {
    PIN_FUNC |= bUART0_PIN_X; // Map UART0 to P1.2 and P1.3
}

/**
 * @brief   Initialise UART0. Timer1 is used as the baud-rate generator by
 *          default (Timer2 may also be used).
 */
inline void mInitSTDIO(void) {
    uint32_t x;
    uint8_t x2;

    SM0 = 0;
    SM1 = 1;
    SM2 = 0;  // Serial port 0 usage mode 1
              // Use Timer1 as a baud rate generator
    RCLK = 0; // UART0 receive clock
    TCLK = 0; // UART0 transmit clock
    PCON |= SMOD;
    x = 10 * FREQ_SYS / UART0_BAUD /
        16; // If the main frequency changes, take care not to overflow x
    x2 = x % 10;
    x /= 10;
    if (x2 >= 5)
        x++; // Rounding

    TMOD = TMOD & ~bT1_GATE & ~bT1_CT & ~MASK_T1_MOD |
           bT1_M1; // 0x20, Timer1 as 8-bit auto-reload timer
    T2MOD = T2MOD | bTMR_CLK | bT1_CLK; // Timer1 clock selection
    TH1 =
        0 -
        x; // 12MHz crystal: baud / 12 is the actual value to set for the baud rate
    TR1 = 1; // Start timer 1
    TI = 1;
    REN = 1; // Serial 0 receive enable
}

/**
 * @brief   Receive one byte from UART0.
 * @return  Received byte (SBUF).
 */
inline uint8_t CH554UART0RcvByte(void) {
    while (RI == 0)
        ; // Wait for UART rx interrupt flag
    RI = 0;
    return SBUF;
}

/**
 * @brief   Send one byte through UART0.
 * @param   SendDat  Byte to transmit.
 */
inline void CH554UART0SendByte(uint8_t SendDat) {

    SBUF = SendDat;
    while (TI == 0)
        ; // Wait for transmit to finish (TI == 1)
    TI = 0;
}

/**
 * @brief   Remap UART1 to alternate pins (TX on P3.2, RX on P3.4).
 */
inline void CH554UART1Alter(void) { PIN_FUNC |= bUART1_PIN_X; }

/**
 * @brief   Initialise UART1.
 */
inline void UART1Setup(void) {
    U1SM0 = 0;  // UART1 8-bit data mode
    U1SMOD = 1; // Fast mode
    U1REN = 1;  // Enable receive
    // Should correct for rounding in SBAUD1 calculation
    SBAUD1 = 256 - FREQ_SYS / 16 / UART1_BAUD;
}

/**
 * @brief   Receive one byte from UART1.
 * @return  Received byte (SBUF1).
 */
inline uint8_t CH554UART1RcvByte(void) {
    while (U1RI == 0)
        ; // Polled receive; not needed in interrupt mode
    U1RI = 0;
    return SBUF1;
}

/**
 * @brief   Send one byte through UART1.
 * @param   SendDat  Byte to transmit.
 */
inline void CH554UART1SendByte(uint8_t SendDat) {
    SBUF1 = SendDat; // Polled transmit; the two lines below are not needed in
                     // interrupt mode, but TI must be 0 before sending.
    while (U1TI == 0)
        ;
    U1TI = 0;
}

#if SDCC < 370
/**
 * @brief   Write one character to UART0 (backing printf for SDCC < 3.70).
 * @param   c  Character to transmit.
 */
void putchar(char c);
/**
 * @brief   Read one character from UART0 (backing getchar for SDCC < 3.70).
 * @return  Received character.
 */
char getchar(void);
#else
/**
 * @brief   Write one character to UART0 (backing printf for SDCC >= 3.70).
 * @param   c  Character to transmit.
 * @return  The transmitted character.
 */
int putchar(int c);
/**
 * @brief   Read one character from UART0 (backing getchar for SDCC >= 3.70).
 * @return  Received character.
 */
int getchar(void);
#endif

/**
 * @brief   Select the watchdog mode (timer or watchdog-reset).
 * @param   mode  0 = use as a timer, 1 = enable watchdog reset.
 */
inline void CH554WDTModeSelect(uint8_t mode) {
    SAFE_MOD = 0x55;
    SAFE_MOD = 0xaa; // Enter Safe Mode
    if (mode) {
        GLOBAL_CFG |= bWDOG_EN; // Start watchdog reset
    }

    else
        GLOBAL_CFG &= ~bWDOG_EN; // Use watchdog only as a timer
    SAFE_MOD = 0x00;             // Exit Safe Mode
    WDOG_COUNT = 0;              // Watchdog counter initial value
}

/**
 * @brief   Feed the watchdog / set the reset time.
 * @param   tim  Watchdog reset-time setting:
 *                 - 0x00 (6MHz): ~2.8 s
 *                 - 0x80 (6MHz): ~1.4 s
 */
inline void CH554WDTFeed(uint8_t tim) {

    WDOG_COUNT = tim; // Watchdog counter assignment
}
