/**
 * @file    adc.c
 * @brief   CH55x ADC sampling clock, channel selection and voltage comparator mode.
 * @author  WCH
 * @version V1.0
 * @date    2019/07/22
 */

#include "adc.h"
#include "ch55x.h"
#include "debug.h"
#include "stdio.h"

#define SUCCESS 1
#define FAIL -1

//uint16_t UserData;

/**
 * @brief   Configure the ADC sampling clock, power on the module and (optionally)
 *          enable the interrupt.
 * @param   speed  ADC clock divider:
 *                   - 0: Slow, 384 * Fosc
 *                   - 1: Fast,  96 * Fosc
 */
void ADCInit(uint8_t speed) {
    ADC_CFG = (ADC_CFG & ~bADC_CLK) | speed;
    ADC_CFG |= bADC_EN; // ADC power enable
#if ADC_INTERRUPT
    ADC_IF = 0; // Clear interrupt
    IE_ADC = 1; // Enable ADC interrupt
#endif
}

/**
 * @brief   Select the ADC sampling channel (0..3).
 * @param   ch  Channel number: 0=AIN0, 1=AIN1, 2=AIN2, 3=AIN3.
 * @return  SUCCESS on valid channel, FAIL otherwise.
 */
uint8_t ADC_ChannelSelect(uint8_t ch) {
    if (ch == 0) {
        ADC_CHAN1 = 0;
        ADC_CHAN0 = 0;
        P1_DIR_PU &= ~bAIN0;
    } // AIN0
    else if (ch == 1) {
        ADC_CHAN1 = 0;
        ADC_CHAN0 = 1;
        P1_DIR_PU &= ~bAIN1;
    } // AIN1
    else if (ch == 2) {
        ADC_CHAN1 = 1;
        ADC_CHAN0 = 0;
        P1_DIR_PU &= ~bAIN2;
    } // AIN2
    else if (ch == 3) {
        ADC_CHAN1 = 1;
        ADC_CHAN0 = 1;
        P3_DIR_PU &= ~bAIN3;
    } // AIN3
    else
        return FAIL;
    return SUCCESS;
}

/**
 * @brief   Initialise the voltage comparator mode.
 * @param   fo  Forward input channel: 0, 1, 2 or 3.
 * @param   re  Reverse input channel: 1 or 3.
 * @return  SUCCESS on valid channel combination, FAIL otherwise.
 */
uint8_t VoltageCMPModeInit(uint8_t fo, uint8_t re) {
    ADC_CFG |= bCMP_EN; // level comparison power enable
    if (re == 1) {
        if (fo == 0) {
            ADC_CHAN1 = 0;
            ADC_CHAN0 = 0;
            CMP_CHAN = 0;
        } // AIN0 and AIN1
        else if (fo == 2) {
            ADC_CHAN1 = 1;
            ADC_CHAN0 = 0;
            CMP_CHAN = 0;
        } // AIN2 and AIN1
        else if (fo == 3) {
            ADC_CHAN1 = 1;
            ADC_CHAN0 = 1;
            CMP_CHAN = 0;
        } // AIN3 and AIN1
        else
            return FAIL;
    } else if (re == 3) {
        if (fo == 0) {
            ADC_CHAN1 = 0;
            ADC_CHAN0 = 0;
            CMP_CHAN = 0;
        } // AIN0 and AIN1
        else if (fo == 1) {
            ADC_CHAN1 = 0;
            ADC_CHAN0 = 1;
            CMP_CHAN = 0;
        } // AIN1 and AIN1
        else if (fo == 2) {
            ADC_CHAN1 = 1;
            ADC_CHAN0 = 0;
            CMP_CHAN = 0;
        } // AIN2 and AIN1
        else
            return FAIL;
    } else
        return FAIL;
#if ADC_INTERRUPT
    CMP_IF = 0; // Clear interrupt
    IE_ADC = 1; // Enable ADC interrupt
#endif

    return SUCCESS;
}

#if ADC_INTERRUPT

/**
 * @brief   ADC interrupt service routine template.
 * @note    This is a template only. Copy it into main.c (and un-comment) when the
 *          ADC is used in interrupt-driven mode.
 */
/*
void	ADCInterrupt(void) __interrupt(INT_NO_ADC)                        //ADC interrupt service routine, using register set 1
{
    if(ADC_IF ==  1)                                                          //ADC complete interrupt
    {
      UserData = ADC_DATA;                                                    //Take ADC sampling data
      ADC_IF = 0;		                                                          //Clear ADC interrupt flag
	  printf(" %d ",UserData);
    }
    if(CMP_IF ==  1)                                                          //Voltage comparison complete interrupt
    {
//       UserData = ADC_CTRL&0x80 >> 7);	                                        //Save comparator result
      CMP_IF = 0;		                                                          //Clear Comparator Complete Interrupt
    }
}
*/
#endif
