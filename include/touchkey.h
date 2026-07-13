/**
 * @file    touchkey.h
 * @brief   CH55x touch-key sampling interval, channel selection/switching and
 *          interrupt handling.
 * @author  WCH
 * @version V1.1
 * @date    2017/07/05
 */
#pragma once

#include <stdint.h>

//#define INTERRUPT_TouchKey   0 // Open TouchKey interrupt mode
#define KEY_FIRST 0 // Sampling start channel
#define KEY_LAST 3  // End of sampling channel
#define KEY_ACT                                                                \
    20 // Button is pressed: the channel sampling value decreases. Lower value = higher sensitivity, higher value = lower sensitivity.
#define KEY_BASE_SAMPLE_TIME                                                   \
    5 // Sampling reference value: number of samples taken to obtain a stable channel reference

extern uint16_t
    KeyFree[KEY_LAST - KEY_FIRST +
            1]; // Touch idle value storage, used to compare the state of the key
extern volatile uint8_t
    KeyBuf; // Touch-key status, 0 means no key, 1 means currently detected key is pressed

/** @brief Disable capacitance detection (for 1ms/2ms timer interrupt only). */
#define TouchKeyOFF()                                                          \
    {                                                                          \
        TKEY_CTRL &= 0xF8;                                                     \
    }
/** @brief Enable capacitance detection but connect no channel. */
#define TouchKeyON_NoChannel()                                                 \
    {                                                                          \
        TKEY_CTRL = TKEY_CTRL & 0xF8 | 7;                                      \
    }
/** @brief Set the touch-key sampling period to 1 ms. */
#define TouchKeyQueryCyl1ms()                                                  \
    { TKEY_CTRL &= ~bTKC_2MS; }
/** @brief Set the touch-key sampling period to 2 ms. */
#define TouchKeyQueryCyl2ms()                                                  \
    { TKEY_CTRL |= bTKC_2MS; }

/**
 * @brief   Select a touch-key sampling channel.
 * @param   ch  Channel selector:
 *                - 0: Disable capacitance detection (1ms/2ms timer interrupt only)
 *                - 1..6: Sampling channel
 *                - 7: Enable capacitance detection, no channel connected
 * @return  1 on success, 0 otherwise.
 */
uint8_t TouchKeyChannelSelect(uint8_t ch);

/**
 * @brief   Sample and store the idle (un-touched) baseline value for every key
 *          channel. Must be called once before polling or enabling interrupts.
 */
void GetTouchKeyFree();

#if !INTERRUPT_TouchKey
/**
 * @brief   Poll the current touch-key channel and update KeyBuf on a press.
 * @note    Blocks until the current sampling cycle (bTKC_IF) completes.
 */
void TouchKeyChannelQuery();
#endif
