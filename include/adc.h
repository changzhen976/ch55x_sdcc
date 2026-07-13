/**
 * @file    adc.h
 * @brief   CH55x ADC sampling clock, channel selection and voltage comparator mode.
 * @author  WCH
 * @version V1.0
 * @date    2019/07/22
 */
#ifndef __ADC_H__
#define __ADC_H__

#include <stdint.h>

/**
 * @brief   Configure the ADC sampling clock, power on the module and (optionally)
 *          enable the interrupt.
 * @param   speed  ADC clock divider:
 *                   - 0: Slow, 384 * Fosc
 *                   - 1: Fast,  96 * Fosc
 */
extern void ADCInit(uint8_t speed);

/**
 * @brief   Select the ADC sampling channel (0..3).
 * @param   ch  Channel number: 0=AIN0, 1=AIN1, 2=AIN2, 3=AIN3.
 * @return  SUCCESS on valid channel, FAIL otherwise.
 */
extern uint8_t ADC_ChannelSelect(uint8_t ch);

/**
 * @brief   Initialise the voltage comparator mode.
 * @param   fo  Forward input channel: 0, 1, 2 or 3.
 * @param   re  Reverse input channel: 1 or 3.
 * @return  SUCCESS on valid channel combination, FAIL otherwise.
 */
extern uint8_t VoltageCMPModeInit(uint8_t fo, uint8_t re);

#endif
