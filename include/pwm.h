/**
 * @file    pwm.h
 * @brief   PWM peripheral driver macros for CH55x.
 * @author  WCH
 * @version V1.0
 * @date    2017/07/05
 */
#pragma once

#if PWM_INTERRUPT
extern void PWMInterruptEnable();
#endif

/** @brief Set the PWM clock divider (default source is Fsys). */
#define SetPWMClk(CK_SE) (PWM_CK_SE = CK_SE)

/** @brief Set the PWM1 output duty cycle. */
#define SetPWM1Dat(dat) (PWM_DATA1 = dat)
/** @brief Set the PWM2 output duty cycle. */
#define SetPWM2Dat(dat) (PWM_DATA2 = dat)

/** @brief Remap the PWM1 output to pin P3.0. */
#define PWM1PinAlter() { PIN_FUNC |= bPWM1_PIN_X; }
/** @brief Remap the PWM2 output to pin P3.1. */
#define PWM2PinAlter() { PIN_FUNC |= bPWM2_PIN_X; }

/** @brief Force-clear the PWM FIFO and COUNT. */
#define ForceClearPWMFIFO() { PWM_CTRL |= bPWM_CLR_ALL; }
/** @brief Cancel the force-clear of the PWM FIFO and COUNT. */
#define CancelClearPWMFIFO() { PWM_CTRL &= ~bPWM_CLR_ALL; }

/** @brief Enable PWM1 output. */
#define PWM1OutEnable() (PWM_CTRL |= bPWM1_OUT_EN)
/** @brief Enable PWM2 output. */
#define PWM2OutEnable() (PWM_CTRL |= bPWM2_OUT_EN)
/** @brief Disable PWM1 output. */
#define DisablePWM1Out() (PWM_CTRL &= ~bPWM1_OUT_EN)
/** @brief Disable PWM2 output. */
#define DisablePWM2Out() (PWM_CTRL &= ~bPWM2_OUT_EN)

/** @brief Set PWM1 output to active-high (default low). */
#define PWM1OutPolarHighAct() (PWM_CTRL &= ~bPWM1_POLAR)
/** @brief Set PWM2 output to active-high (default low). */
#define PWM2OutPolarHighAct() (PWM_CTRL &= ~bPWM2_POLAR)
/** @brief Set PWM1 output to active-low (default high). */
#define PWM1OutPolarLowAct() (PWM_CTRL |= bPWM1_POLAR)
/** @brief Set PWM2 output to active-low (default high). */
#define PWM2OutPolarLowAct() (PWM_CTRL |= bPWM2_POLAR)

/** @brief Enable the PWM end-of-cycle interrupt. */
#define PWMInterruptEnable()                \
    {                                       \
        PWM_CTRL |= bPWM_IF_END | bPWM_IE_END; \
        IE_PWMX = 1;                        \
    }
/** @brief Disable the PWM interrupt. */
#define PWMInterruptDisable() { IE_PWMX = 0; }
