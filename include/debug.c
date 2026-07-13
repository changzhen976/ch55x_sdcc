/**
 * @file    debug.c
 * @brief   CH55x debug interface: main-frequency configuration, delay routines,
 *          UART0/UART1 setup and putchar/getchar for printf.
 * @author  WCH
 * @version V1.0
 * @date    2017/01/20
 */

#include <stdint.h>

#include "ch55x.h"
#include "debug.h"

/**
 * @brief   Select and configure the CH55x system clock (Fsys).
 *
 * Fsys defaults to 6 MHz; it is derived from FREQ_SYS via CLOCK_CFG:
 *   Fsys = (Fosc * 4) / (CLOCK_CFG & MASK_SYS_CK_SEL)
 * Set FREQ_SYS to the desired frequency before calling this function.
 */
void CfgFsys(void) {
    SAFE_MOD = 0x55;
    SAFE_MOD = 0xAA;
    //     CLOCK_CFG |= bOSC_EN_XT;                          // Enable external crystal
    //     CLOCK_CFG & = ~ bOSC_EN_INT;                      // Turn off the internal crystal

#if FREQ_SYS == 32000000
    CLOCK_CFG = CLOCK_CFG & ~MASK_SYS_CK_SEL | 0x07; // 32MHz
#elif FREQ_SYS == 24000000
    CLOCK_CFG = CLOCK_CFG & ~MASK_SYS_CK_SEL | 0x06; // 24MHz
#elif FREQ_SYS == 16000000
    CLOCK_CFG = CLOCK_CFG & ~MASK_SYS_CK_SEL | 0x05; // 16MHz
#elif FREQ_SYS == 12000000
    CLOCK_CFG = CLOCK_CFG & ~MASK_SYS_CK_SEL | 0x04; // 12MHz
#elif FREQ_SYS == 6000000
    CLOCK_CFG = CLOCK_CFG & ~MASK_SYS_CK_SEL | 0x03; // 6MHz
#elif FREQ_SYS == 3000000
    CLOCK_CFG = CLOCK_CFG & ~MASK_SYS_CK_SEL | 0x02; // 3MHz
#elif FREQ_SYS == 750000
    CLOCK_CFG = CLOCK_CFG & ~MASK_SYS_CK_SEL | 0x01; // 750KHz
#elif FREQ_SYS == 187500
    CLOCK_CFG = CLOCK_CFG & ~MASK_SYS_CK_SEL | 0x00; // 187.5MHz
#else
#warning FREQ_SYS invalid or not set
#endif

    SAFE_MOD = 0x00;
}

/**
 * @brief   Microsecond delay.
 * @param   n  Number of microseconds to wait (calibrated to ~1us @ Fsys=12MHz).
 */
void mDelayuS(uint16_t n) {
#ifdef FREQ_SYS
#if FREQ_SYS <= 6000000
    n >>= 2;
#endif
#if FREQ_SYS <= 3000000
    n >>= 2;
#endif
#if FREQ_SYS <= 750000
    n >>= 4;
#endif
#endif
    while (n) {     // total = 12~13 Fsys cycles, 1uS @Fsys=12MHz
        ++SAFE_MOD; // 2 Fsys cycles, for higher Fsys, add operation here
#ifdef FREQ_SYS
#if FREQ_SYS >= 14000000
        ++SAFE_MOD;
#endif
#if FREQ_SYS >= 16000000
        ++SAFE_MOD;
#endif
#if FREQ_SYS >= 18000000
        ++SAFE_MOD;
#endif
#if FREQ_SYS >= 20000000
        ++SAFE_MOD;
#endif
#if FREQ_SYS >= 22000000
        ++SAFE_MOD;
#endif
#if FREQ_SYS >= 24000000
        ++SAFE_MOD;
#endif
#if FREQ_SYS >= 26000000
        ++SAFE_MOD;
#endif
#if FREQ_SYS >= 28000000
        ++SAFE_MOD;
#endif
#if FREQ_SYS >= 30000000
        ++SAFE_MOD;
#endif
#if FREQ_SYS >= 32000000
        ++SAFE_MOD;
#endif
#endif
        --n;
    }
}

/**
 * @brief   Millisecond delay.
 * @param   n  Number of milliseconds to wait.
 */
void mDelaymS(uint16_t n) {
    while (n) {
#ifdef DELAY_MS_HW
        while ((TKEY_CTRL & bTKC_IF) == 0)
            ;
        while (TKEY_CTRL & bTKC_IF)
            ;
#else
        mDelayuS(1000);
#endif
        --n;
    }
}

#if SDCC < 370
/**
 * @brief   Write one character to UART0 (backing printf for SDCC < 3.70).
 * @param   c  Character to transmit.
 */
void putchar(char c) {
    while (!TI)
        ; // assumes UART is initialized
    TI = 0;
    SBUF = c;
}

/**
 * @brief   Read one character from UART0 (backing getchar for SDCC < 3.70).
 * @return  Received character.
 */
char getchar(void) {
    while (!RI)
        ; // assumes UART is initialized
    RI = 0;
    return SBUF;
}
#else
/**
 * @brief   Write one character to UART0 (backing printf for SDCC >= 3.70).
 * @param   c  Character to transmit.
 * @return  The transmitted character.
 */
int putchar(int c) {
    while (!TI)
        ; // assumes UART is initialized
    TI = 0;
    SBUF = c & 0xFF;

    return c;
}

/**
 * @brief   Read one character from UART0 (backing getchar for SDCC >= 3.70).
 * @return  Received character.
 */
int getchar(void) {
    while (!RI)
        ; // assumes UART is initialized
    RI = 0;
    return SBUF;
}
#endif
