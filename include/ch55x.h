/*--------------------------------------------------------------------------
CH55x.H
Header file for CH55x microcontrollers (CH552/CH554).
****************************************
**  Copyright  (C)  W.ch  1999-2014   **
**  Web:              http://wch.cn   **
****************************************
--------------------------------------------------------------------------*/

#ifndef __CH55X_H__
#define __CH55X_H__

#include <compiler.h>

/*----- SFR --------------------------------------------------------------*/
/*  sbit are bit addressable, others are byte addressable */

/*  System Registers  */
/** @brief Program status word. */
SFR(PSW, 0xD0);
/** @brief Carry flag. */
SBIT(CY, 0xD0, 7);
/** @brief Auxiliary carry flag. */
SBIT(AC, 0xD0, 6);
/** @brief Bit addressable general purpose flag 0. */
SBIT(F0, 0xD0, 5);
/** @brief Register R0-R7 bank selection high bit. */
SBIT(RS1, 0xD0, 4);
/** @brief Register R0-R7 bank selection low bit. */
SBIT(RS0, 0xD0, 3);
#define MASK_PSW_RS                                                            \
    0x18 /* bit mask of register R0-R7 bank selection
             RS1 & RS0: register R0-R7 bank selection
                00 - bank 0, R0-R7 @ address 0x00-0x07
                01 - bank 1, R0-R7 @ address 0x08-0x0F
                10 - bank 2, R0-R7 @ address 0x10-0x17
                11 - bank 3, R0-R7 @ address 0x18-0x1F */
/** @brief Overflow flag. */
SBIT(OV, 0xD0, 2);
/** @brief Bit addressable general purpose flag 1. */
SBIT(F1, 0xD0, 1);
/** @brief ReadOnly: parity flag. */
SBIT(P, 0xD0, 0);
/** @brief Accumulator. */
SFR(ACC, 0xE0);
/** @brief General purpose register B. */
SFR(B, 0xF0);
/** @brief Stack pointer. */
SFR(SP, 0x81);
//sfr16 DPTR          = 0x82;         // DPTR pointer, little-endian
/** @brief Data pointer low. */
SFR(DPL, 0x82);
/** @brief Data pointer high. */
SFR(DPH, 0x83);
/** @brief WriteOnly: writing safe mode. */
SFR(SAFE_MOD, 0xA1);
//sfr CHIP_ID         = 0xA1;         // ReadOnly: reading chip ID
#define CHIP_ID SAFE_MOD
/** @brief Global config, Write@SafeMode. */
SFR(GLOBAL_CFG, 0xB1);
/** @brief ReadOnly: boot loader status for discriminating BootLoader or Application: set 1 by power on reset, clear 0 by software reset. */
#define bBOOT_LOAD                                                             \
    0x20
/** @brief Software reset bit, auto clear by hardware. */
#define bSW_RESET 0x10
/** @brief Enable flash-ROM (include code & Data-Flash) being program or erasing: 0=writing protect, 1=enable program and erase. */
#define bCODE_WE                                                               \
    0x08
/** @brief Enable Data-Flash (flash-ROM data area) being program or erasing: 0=writing protect, 1=enable program and erase. */
#define bDATA_WE                                                               \
    0x04
/** @brief Disable 5V->3.3V LDO: 0=enable LDO for USB and internal oscillator under 5V power, 1=disable LDO, V33 pin input external 3.3V power. */
#define bLDO3V3_OFF                                                            \
    0x02
/** @brief Enable watch-dog reset if watch-dog timer overflow: 0=as timer only, 1=enable reset if timer overflow. */
#define bWDOG_EN                                                               \
    0x01

/* Clock and Sleep and Power Registers */
/** @brief Power control and reset flag. */
SFR(PCON, 0x87);
#define SMOD                                                                   \
    0x80 /* baud rate selection for UART0 mode 1/2/3:
             0=slow(Fsys/128 @mode2, TF1/32 @mode1/3, no effect for TF2),
             1=fast(Fsys/32 @mode2, TF1/16 @mode1/3, no effect for TF2) */
/** @brief ReadOnly: recent reset flag high bit. */
#define bRST_FLAG1 0x20
/** @brief ReadOnly: recent reset flag low bit. */
#define bRST_FLAG0 0x10
/**
 * @brief (ReadOnly) Bit mask of recent reset flag.
 *
 * Recent reset source (bRST_FLAG1 & bRST_FLAG0):
 * - 00: software reset (bSW_RESET=1)
 * - 01: power-on reset
 * - 10: watchdog timer overflow
 * - 11: external RST pin
 */
#define MASK_RST_FLAG 0x30
#define RST_FLAG_SW 0x00
#define RST_FLAG_POR 0x10
#define RST_FLAG_WDOG 0x20
#define RST_FLAG_PIN 0x30
/** @brief General purpose flag bit 1. */
#define GF1 0x08
/** @brief General purpose flag bit 0. */
#define GF0 0x04
/** @brief Power-down enable bit, auto clear by wake-up hardware. */
#define PD 0x02
/** @brief System clock config: lower 3 bits for system clock Fsys, Write@SafeMode. */
SFR(CLOCK_CFG,
    0xB9);
/** @brief Internal oscillator enable and original clock selection: 1=enable & select internal clock, 0=disable & select external clock. */
#define bOSC_EN_INT                                                            \
    0x80
/** @brief External oscillator enable, need quartz crystal or ceramic resonator between XI and XO pins. */
#define bOSC_EN_XT                                                             \
    0x40
/** @brief ReadOnly: watch-dog timer overflow interrupt flag, cleared by reload watch-dog count or auto cleared when MCU enter interrupt routine. */
#define bWDOG_IF_TO                                                            \
    0x20
/** @brief Flash-ROM clock frequency selection: 0=normal(for Fosc>=16MHz), 1=fast(for Fosc<16MHz). */
#define bROM_CLK_FAST                                                          \
    0x10
/** @brief ReadOnly: pin RST input. */
#define bRST 0x08
/** @brief Alternate pin for T2EX. */
#define bT2EX_ 0x08
/** @brief Alternate pin for CAP2. */
#define bCAP2_ 0x08
/** @brief Bit mask of system clock Fsys selection. */
#define MASK_SYS_CK_SEL 0x07
/*
   Fxt = 24MHz(8MHz~25MHz for non-USB application), from external oscillator @XI&XO
   Fosc = bOSC_EN_INT ? 24MHz : Fxt
   Fpll = Fosc * 4 => 96MHz (32MHz~100MHz for non-USB application)
   Fusb4x = Fpll / 2 => 48MHz (Fixed)
              MASK_SYS_CK_SEL[2] [1] [0]
   Fsys = Fpll/3   =  32MHz:  1   1   1
   Fsys = Fpll/4   =  24MHz:  1   1   0
   Fsys = Fpll/6   =  16MHz:  1   0   1
   Fsys = Fpll/8   =  12MHz:  1   0   0
   Fsys = Fpll/16  =   6MHz:  0   1   1
   Fsys = Fpll/32  =   3MHz:  0   1   0
   Fsys = Fpll/128 = 750KHz:  0   0   1
   Fsys = Fpll/512 =187.5KHz: 0   0   0
*/
/** @brief Wake-up control, Write@SafeMode. */
SFR(WAKE_CTRL, 0xA9);
/** @brief Enable wake-up by USB event. */
#define bWAK_BY_USB 0x80
/** @brief Enable wake-up by RXD1 low level. */
#define bWAK_RXD1_LO 0x40
/** @brief Enable wake-up by pin P1.5 low level. */
#define bWAK_P1_5_LO 0x20
/** @brief Enable wake-up by pin P1.4 low level. */
#define bWAK_P1_4_LO 0x10
/** @brief Enable wake-up by pin P1.3 low level. */
#define bWAK_P1_3_LO 0x08
/** @brief Enable wake-up by pin RST high level. */
#define bWAK_RST_HI 0x04
/** @brief Enable wake-up by pin P3.2 (INT0) edge or pin P3.3 (INT1) low level. */
#define bWAK_P3_2E_3L                                                          \
    0x02
/** @brief Enable wake-up by RXD0 low level. */
#define bWAK_RXD0_LO 0x01
/** @brief Value keeper during reset. */
SFR(RESET_KEEP, 0xFE);
/** @brief Watch-dog count, count by clock frequency Fsys/65536. */
SFR(WDOG_COUNT, 0xFF);

/*  Interrupt Registers  */
/** @brief Interrupt enable. */
SFR(IE, 0xA8);
/** @brief Enable global interrupts: 0=disable, 1=enable if E_DIS=0. */
SBIT(EA, 0xA8, 7);
/** @brief Disable global interrupts, intend to inhibit interrupt during some flash-ROM operation: 0=enable if EA=1, 1=disable. */
SBIT(
    E_DIS, 0xA8,
    6);
/** @brief Enable timer2 interrupt. */
SBIT(ET2, 0xA8, 5);
/** @brief Enable UART0 interrupt. */
SBIT(ES, 0xA8, 4);
/** @brief Enable timer1 interrupt. */
SBIT(ET1, 0xA8, 3);
/** @brief Enable external interrupt INT1. */
SBIT(EX1, 0xA8, 2);
/** @brief Enable timer0 interrupt. */
SBIT(ET0, 0xA8, 1);
/** @brief Enable external interrupt INT0. */
SBIT(EX0, 0xA8, 0);
/** @brief Interrupt priority and current priority. */
SFR(IP, 0xB8);
/** @brief ReadOnly: high level priority action flag. */
SBIT(PH_FLAG, 0xB8, 7);
/** @brief ReadOnly: low level priority action flag. */
SBIT(PL_FLAG, 0xB8, 6);
                         // PH_FLAG & PL_FLAG: current interrupt priority
                         //    00 - no interrupt now
                         //    01 - low level priority interrupt action now
                         //    10 - high level priority interrupt action now
                         //    11 - unknown error
/** @brief Timer2 interrupt priority level. */
SBIT(PT2, 0xB8, 5);
/** @brief UART0 interrupt priority level. */
SBIT(PS, 0xB8, 4);
/** @brief Timer1 interrupt priority level. */
SBIT(PT1, 0xB8, 3);
/** @brief External interrupt INT1 priority level. */
SBIT(PX1, 0xB8, 2);
/** @brief Timer0 interrupt priority level. */
SBIT(PT0, 0xB8, 1);
/** @brief External interrupt INT0 priority level. */
SBIT(PX0, 0xB8, 0);
/** @brief Extend interrupt enable. */
SFR(IE_EX, 0xE8);
/** @brief Enable watch-dog timer interrupt. */
SBIT(IE_WDOG, 0xE8, 7);
/** @brief Enable GPIO input interrupt. */
SBIT(IE_GPIO, 0xE8, 6);
/** @brief Enable PWM1/2 interrupt. */
SBIT(IE_PWMX, 0xE8, 5);
/** @brief Enable UART1 interrupt. */
SBIT(IE_UART1, 0xE8, 4);
/** @brief Enable ADC interrupt. */
SBIT(IE_ADC, 0xE8, 3);
/** @brief Enable USB interrupt. */
SBIT(IE_USB, 0xE8, 2);
/** @brief Enable touch-key timer interrupt. */
SBIT(IE_TKEY, 0xE8, 1);
/** @brief Enable SPI0 interrupt. */
SBIT(IE_SPI0, 0xE8, 0);
/** @brief Extend interrupt priority. */
SFR(IP_EX, 0xE9);
/** @brief ReadOnly: current interrupt nested level: 0=no interrupt or two levels, 1=one level. */
#define bIP_LEVEL                                                              \
    0x80
/** @brief GPIO input interrupt priority level. */
#define bIP_GPIO 0x40
/** @brief PWM1/2 interrupt priority level. */
#define bIP_PWMX 0x20
/** @brief UART1 interrupt priority level. */
#define bIP_UART1 0x10
/** @brief ADC interrupt priority level. */
#define bIP_ADC 0x08
/** @brief USB interrupt priority level. */
#define bIP_USB 0x04
/** @brief Touch-key timer interrupt priority level. */
#define bIP_TKEY 0x02
/** @brief SPI0 interrupt priority level. */
#define bIP_SPI0 0x01
/** @brief GPIO interrupt enable. */
SFR(GPIO_IE, 0xC7);
/** @brief Enable GPIO edge interrupt: 0=low/high level, 1=falling/rising edge. */
#define bIE_IO_EDGE                                                            \
    0x80
/** @brief Enable interrupt by RXD1 low level / falling edge. */
#define bIE_RXD1_LO 0x40
/** @brief Enable interrupt by pin P1.5 low level / falling edge. */
#define bIE_P1_5_LO                                                            \
    0x20
/** @brief Enable interrupt by pin P1.4 low level / falling edge. */
#define bIE_P1_4_LO                                                            \
    0x10
/** @brief Enable interrupt by pin P1.3 low level / falling edge. */
#define bIE_P1_3_LO                                                            \
    0x08
/** @brief Enable interrupt by pin RST high level / rising edge. */
#define bIE_RST_HI 0x04
/** @brief Enable interrupt by pin P3.1 low level / falling edge. */
#define bIE_P3_1_LO                                                            \
    0x02
/** @brief Enable interrupt by RXD0 low level / falling edge. */
#define bIE_RXD0_LO 0x01

/*  FlashROM and Data-Flash Registers  */
/** @brief Address for flash-ROM, little-endian. */
SFR16(ROM_ADDR, 0x84);
/** @brief Address low byte for flash-ROM. */
SFR(ROM_ADDR_L, 0x84);
/** @brief Address high byte for flash-ROM. */
SFR(ROM_ADDR_H, 0x85);
/** @brief Data for flash-ROM writing, little-endian. */
SFR16(ROM_DATA, 0x8E);
/** @brief Data low byte for flash-ROM writing, data byte for Data-Flash reading/writing. */
SFR(ROM_DATA_L,
    0x8E);
/** @brief Data high byte for flash-ROM writing. */
SFR(ROM_DATA_H, 0x8F);
/** @brief WriteOnly: flash-ROM control. */
SFR(ROM_CTRL, 0x86);
/** @brief WriteOnly: flash-ROM word or Data-Flash byte write operation command. */
#define ROM_CMD_WRITE                                                          \
    0x9A
/** @brief WriteOnly: Data-Flash byte read operation command. */
#define ROM_CMD_READ 0x8E
//sfr ROM_STATUS      = 0x86;         // ReadOnly: flash-ROM status
#define ROM_STATUS ROM_CTRL
/** @brief ReadOnly: flash-ROM writing operation address valid flag, can be reviewed before or after operation: 0=invalid parameter, 1=address valid. */
#define bROM_ADDR_OK                                                           \
    0x40
/** @brief ReadOnly: flash-ROM operation command error flag: 0=command accepted, 1=unknown command. */
#define bROM_CMD_ERR                                                           \
    0x02

/*  Port Registers  */
/** @brief Port 1 input & output. */
SFR(P1, 0x90);
/** @brief Serial clock for SPI0. */
SBIT(SCK, 0x90, 7);
/** @brief TXD output for UART1. */
SBIT(TXD1, 0x90, 7);
/** @brief TIN5 for Touch-Key. */
SBIT(TIN5, 0x90, 7);
/** @brief Master serial data input or slave serial data output for SPI0. */
SBIT(MISO, 0x90,
     6);
/** @brief RXD input for UART1. */
SBIT(RXD1, 0x90, 6);
/** @brief TIN4 for Touch-Key. */
SBIT(TIN4, 0x90, 6);
/** @brief Master serial data output or slave serial data input for SPI0. */
SBIT(MOSI, 0x90,
     5);
/** @brief PWM output for PWM1. */
SBIT(PWM1, 0x90, 5);
/** @brief TIN3 for Touch-Key. */
SBIT(TIN3, 0x90, 5);
/** @brief CC2 for USB type-C. */
SBIT(UCC2, 0x90, 5);
/** @brief AIN2 for ADC. */
SBIT(AIN2, 0x90, 5);
/** @brief Alternate pin for T2. */
SBIT(T2_, 0x90, 4);
/** @brief Alternate pin for CAP1. */
SBIT(CAP1_, 0x90, 4);
/** @brief Slave chip-selection input for SPI0. */
SBIT(SCS, 0x90, 4);
/** @brief TIN2 for Touch-Key. */
SBIT(TIN2, 0x90, 4);
/** @brief CC1 for USB type-C. */
SBIT(UCC1, 0x90, 4);
/** @brief AIN1 for ADC. */
SBIT(AIN1, 0x90, 4);
/** @brief Alternate pin for TXD of UART0. */
SBIT(TXD_, 0x90, 3);
/** @brief Alternate pin for RXD of UART0. */
SBIT(RXD_, 0x90, 2);
/** @brief External trigger input for timer2 reload & capture. */
SBIT(T2EX, 0x90, 1);
/** @brief Capture2 input for timer2. */
SBIT(CAP2, 0x90, 1);
/** @brief TIN1 for Touch-Key. */
SBIT(TIN1, 0x90, 1);
/** @brief VBUS2 for USB type-C. */
SBIT(VBUS2, 0x90, 1);
/** @brief AIN0 for ADC. */
SBIT(AIN0, 0x90, 1);
/** @brief External count input. */
SBIT(T2, 0x90, 0);
/** @brief Capture1 input for timer2. */
SBIT(CAP1, 0x90, 0);
/** @brief TIN0 for Touch-Key. */
SBIT(TIN0, 0x90, 0);
/**
 * @brief Port 1 output mode: 0=push-pull, 1=open-drain.
 *
 * Pin configuration (Pn_MOD_OC & Pn_DIR_PU for n=1/3):
 * - 00: float input only, no pull-up
 * - 01: push-pull output
 * - 10: open-drain output/input, no pull-up
 * - 11: quasi-bidirectional (8051), internal pull-up, momentary strong high drive
 */
SFR(P1_MOD_OC, 0x92);
/** @brief Port 1 direction for push-pull or pullup enable for open-drain. */
SFR(P1_DIR_PU,
    0x93);
/** @brief Serial clock for SPI0. */
#define bSCK 0x80
/** @brief TXD output for UART1. */
#define bTXD1 0x80
/** @brief Master serial data input or slave serial data output for SPI0. */
#define bMISO                                                                  \
    0x40
/** @brief RXD input for UART1. */
#define bRXD1 0x40
/** @brief Master serial data output or slave serial data input for SPI0. */
#define bMOSI                                                                  \
    0x20
/** @brief PWM output for PWM1. */
#define bPWM1 0x20
/** @brief CC2 for USB type-C. */
#define bUCC2 0x20
/** @brief AIN2 for ADC. */
#define bAIN2 0x20
/** @brief Alternate pin for T2. */
#define bT2_ 0x10
/** @brief Alternate pin for CAP1. */
#define bCAP1_ 0x10
/** @brief Slave chip-selection input for SPI0. */
#define bSCS 0x10
/** @brief CC1 for USB type-C. */
#define bUCC1 0x10
/** @brief AIN1 for ADC. */
#define bAIN1 0x10
/** @brief Alternate pin for TXD of UART0. */
#define bTXD_ 0x08
/** @brief Alternate pin for RXD of UART0. */
#define bRXD_ 0x04
/** @brief External trigger input for timer2 reload & capture. */
#define bT2EX 0x02
/** @brief Capture2 input for timer2. */
#define bCAP2 bT2EX
/** @brief VBUS2 for USB type-C. */
#define bVBUS2 0x02
/** @brief AIN0 for ADC. */
#define bAIN0 0x02
/** @brief External count input or clock output for timer2. */
#define bT2 0x01
/** @brief Capture1 input for timer2. */
#define bCAP1 bT2
/** @brief Port 2. */
SFR(P2, 0xA0);
/** @brief Port 3 input & output. */
SFR(P3, 0xB0);
/** @brief ReadOnly: pin UDM input. */
SBIT(UDM, 0xB0, 7);
/** @brief ReadOnly: pin UDP input. */
SBIT(UDP, 0xB0, 6);
/** @brief External count input for timer1. */
SBIT(T1, 0xB0, 5);
/** @brief PWM output for PWM2. */
SBIT(PWM2, 0xB0, 4);
/** @brief Alternate pin for RXD1. */
SBIT(RXD1_, 0xB0, 4);
/** @brief External count input for timer0. */
SBIT(T0, 0xB0, 4);
/** @brief External interrupt 1 input. */
SBIT(INT1, 0xB0, 3);
/** @brief Alternate pin for TXD1. */
SBIT(TXD1_, 0xB0, 2);
/** @brief External interrupt 0 input. */
SBIT(INT0, 0xB0, 2);
/** @brief VBUS1 for USB type-C. */
SBIT(VBUS1, 0xB0, 2);
/** @brief AIN3 for ADC. */
SBIT(AIN3, 0xB0, 2);
/** @brief Alternate pin for PWM2. */
SBIT(PWM2_, 0xB0, 1);
/** @brief TXD output for UART0. */
SBIT(TXD, 0xB0, 1);
/** @brief Alternate pin for PWM1. */
SBIT(PWM1_, 0xB0, 0);
/** @brief RXD input for UART0. */
SBIT(RXD, 0xB0, 0);
/** @brief Port 3 output mode: 0=push-pull, 1=open-drain. */
SFR(P3_MOD_OC, 0x96);
/** @brief Port 3 direction for push-pull or pullup enable for open-drain. */
SFR(P3_DIR_PU,
    0x97);
/** @brief ReadOnly: pin UDM input. */
#define bUDM 0x80
/** @brief ReadOnly: pin UDP input. */
#define bUDP 0x40
/** @brief External count input for timer1. */
#define bT1 0x20
/** @brief PWM output for PWM2. */
#define bPWM2 0x10
/** @brief Alternate pin for RXD1. */
#define bRXD1_ 0x10
/** @brief External count input for timer0. */
#define bT0 0x10
/** @brief External interrupt 1 input. */
#define bINT1 0x08
/** @brief Alternate pin for TXD1. */
#define bTXD1_ 0x04
/** @brief External interrupt 0 input. */
#define bINT0 0x04
/** @brief VBUS1 for USB type-C. */
#define bVBUS1 0x04
/** @brief AIN3 for ADC. */
#define bAIN3 0x04
/** @brief Alternate pin for PWM2. */
#define bPWM2_ 0x02
/** @brief TXD output for UART0. */
#define bTXD 0x02
/** @brief Alternate pin for PWM1. */
#define bPWM1_ 0x01
/** @brief RXD input for UART0. */
#define bRXD 0x01
/** @brief Pin function selection. */
SFR(PIN_FUNC, 0xC6);
/** @brief USB UDP/UDM I/O pin enable: 0=P3.6/P3.7 as GPIO, 1=P3.6/P3.7 as USB. */
#define bUSB_IO_EN                                                             \
    0x80
/** @brief ReadOnly: GPIO interrupt request action status. */
#define bIO_INT_ACT 0x40
/** @brief UART1 alternate pin enable: 0=RXD1/TXD1 on P1.6/P1.7, 1=RXD1/TXD1 on P3.4/P3.2. */
#define bUART1_PIN_X                                                           \
    0x20
/** @brief UART0 alternate pin enable: 0=RXD0/TXD0 on P3.0/P3.1, 1=RXD0/TXD0 on P1.2/P1.3. */
#define bUART0_PIN_X                                                           \
    0x10
/** @brief PWM2 alternate pin enable: 0=PWM2 on P3.4, 1=PWM2 on P3.1. */
#define bPWM2_PIN_X                                                            \
    0x08
/** @brief PWM1 alternate pin enable: 0=PWM1 on P1.5, 1=PWM1 on P3.0. */
#define bPWM1_PIN_X                                                            \
    0x04
/** @brief T2EX/CAP2 alternate pin enable: 0=T2EX/CAP2 on P1.1, 1=T2EX/CAP2 on RST. */
#define bT2EX_PIN_X                                                            \
    0x02
/** @brief T2/CAP1 alternate pin enable: 0=T2/CAP1 on P1.1, 1=T2/CAP1 on P1.4. */
#define bT2_PIN_X                                                              \
    0x01
/** @brief XBUS auxiliary setting. */
SFR(XBUS_AUX, 0xA2);
/** @brief ReadOnly: indicate UART0 transmittal status. */
#define bUART0_TX 0x80
/** @brief ReadOnly: indicate UART0 receiving status. */
#define bUART0_RX 0x40
/** @brief ReadOnly: safe mode action status. */
#define bSAFE_MOD_ACT 0x20
/** @brief General purpose flag bit 2. */
#define GF2 0x08
/** @brief Enable DPTR auto increase if finished MOVX_@DPTR instruction. */
#define bDPTR_AUTO_INC                                                         \
    0x04
/** @brief Dual DPTR selection: 0=DPTR0 selected, 1=DPTR1 selected. */
#define DPS 0x01

/*  Timer0/1 Registers  */
/** @brief Timer 0/1 control and external interrupt control. */
SFR(TCON, 0x88);
/** @brief Timer1 overflow & interrupt flag, auto cleared when MCU enter interrupt routine. */
SBIT(
    TF1, 0x88,
    7);
/** @brief Timer1 run enable. */
SBIT(TR1, 0x88, 6);
/** @brief Timer0 overflow & interrupt flag, auto cleared when MCU enter interrupt routine. */
SBIT(
    TF0, 0x88,
    5);
/** @brief Timer0 run enable. */
SBIT(TR0, 0x88, 4);
/** @brief INT1 interrupt flag, auto cleared when MCU enter interrupt routine. */
SBIT(IE1, 0x88,
     3);
/** @brief INT1 interrupt type: 0=low level action, 1=falling edge action. */
SBIT(IT1, 0x88,
     2);
/** @brief INT0 interrupt flag, auto cleared when MCU enter interrupt routine. */
SBIT(IE0, 0x88,
     1);
/** @brief INT0 interrupt type: 0=low level action, 1=falling edge action. */
SBIT(IT0, 0x88,
     0);
/** @brief Timer 0/1 mode. */
SFR(TMOD, 0x89);
/** @brief Gate control of timer1: 0=timer1 run enable while TR1=1, 1=timer1 run enable while P3.3 (INT1) pin is high and TR1=1. */
#define bT1_GATE                                                               \
    0x80
/** @brief Counter or timer mode selection for timer1: 0=timer, use internal clock, 1=counter, use P3.5 (T1) pin falling edge as clock. */
#define bT1_CT                                                                 \
    0x40
/** @brief Timer1 mode high bit. */
#define bT1_M1 0x20
/** @brief Timer1 mode low bit. */
#define bT1_M0 0x10
/**
 * @brief Bit mask of timer1 mode (bT1_M1 & bT1_M0).
 *
 * - 00: mode 0, 13-bit timer/counter (TH1 + lower 5 bits of TL1)
 * - 01: mode 1, 16-bit timer/counter (TH1 + TL1)
 * - 10: mode 2, 8-bit auto-reload (TL1 counter, TH1 reload value)
 * - 11: mode 3, stop timer1
 */
#define MASK_T1_MOD 0x30
/** @brief Gate control of timer0: 0=timer0 run enable while TR0=1, 1=timer0 run enable while P3.2 (INT0) pin is high and TR0=1. */
#define bT0_GATE                                                               \
    0x08
/** @brief Counter or timer mode selection for timer0: 0=timer, use internal clock, 1=counter, use P3.4 (T0) pin falling edge as clock. */
#define bT0_CT                                                                 \
    0x04
/** @brief Timer0 mode high bit. */
#define bT0_M1 0x02
/** @brief Timer0 mode low bit. */
#define bT0_M0 0x01
/**
 * @brief Bit mask of timer0 mode (bT0_M1 & bT0_M0).
 *
 * - 00: mode 0, 13-bit timer/counter (TH0 + lower 5 bits of TL0)
 * - 01: mode 1, 16-bit timer/counter (TH0 + TL0)
 * - 10: mode 2, 8-bit auto-reload (TL0 counter, TH0 reload value)
 * - 11: mode 3, split timer (TL0 8-bit, TH0 8-bit using TF1)
 */
#define MASK_T0_MOD 0x03
/** @brief Low byte of timer 0 count. */
SFR(TL0, 0x8A);
/** @brief Low byte of timer 1 count. */
SFR(TL1, 0x8B);
/** @brief High byte of timer 0 count. */
SFR(TH0, 0x8C);
/** @brief High byte of timer 1 count. */
SFR(TH1, 0x8D);

/*  UART0 Registers  */
/** @brief UART0 control (serial port control). */
SFR(SCON, 0x98);
/** @brief UART0 mode bit0, selection data bit: 0=8 bits data, 1=9 bits data. */
SBIT(SM0, 0x98,
     7);
/** @brief UART0 mode bit1, selection baud rate: 0=fixed, 1=variable. */
SBIT(SM1, 0x98, 6);
/** @brief Enable multi-device communication in mode 2/3. */
SBIT(SM2, 0x98, 5);
/**
 * @brief Bit mask of UART0 mode (SM0 & SM1).
 *
 * - 00: mode 0, shift register, Fsys/12
 * - 01: mode 1, 8-bit UART, variable baud
 * - 10: mode 2, 9-bit UART, Fsys/128 or Fsys/32
 * - 11: mode 3, 9-bit UART, variable baud
 */
#define MASK_UART0_MOD 0xE0
/** @brief Enable UART0 receiving. */
SBIT(REN, 0x98, 4);
/** @brief The 9th transmitted data bit in mode 2/3. */
SBIT(TB8, 0x98, 3);
/** @brief 9th data bit received in mode 2/3, or stop bit received for mode 1. */
SBIT(RB8, 0x98,
     2);
/** @brief Transmit interrupt flag, set by hardware after completion of a serial transmittal, need software clear. */
SBIT(
    TI, 0x98,
    1);
/** @brief Receive interrupt flag, set by hardware after completion of a serial receiving, need software clear. */
SBIT(
    RI, 0x98,
    0);
/** @brief UART0 data buffer: reading for receiving, writing for transmittal. */
SFR(SBUF,
    0x99);

/*  Timer2/Capture2 Registers  */
/** @brief Timer 2 control. */
SFR(T2CON, 0xC8);
/** @brief Timer2 overflow & interrupt flag, need software clear, the flag will not be set when either RCLK=1 or TCLK=1. */
SBIT(
    TF2, 0xC8,
    7);
/** @brief Timer2 capture 1 interrupt flag, set by T2 edge trigger if bT2_CAP1_EN=1, need software clear. */
SBIT(
    CAP1F, 0xC8,
    7);
/** @brief Timer2 external flag, set by T2EX edge trigger if EXEN2=1, need software clear. */
SBIT(
    EXF2, 0xC8,
    6);
/** @brief Selection UART0 receiving clock: 0=timer1 overflow pulse, 1=timer2 overflow pulse. */
SBIT(
    RCLK, 0xC8,
    5);
/** @brief Selection UART0 transmittal clock: 0=timer1 overflow pulse, 1=timer2 overflow pulse. */
SBIT(
    TCLK, 0xC8,
    4);
/** @brief Enable T2EX trigger function: 0=ignore T2EX, 1=trigger reload or capture by T2EX edge. */
SBIT(
    EXEN2, 0xC8,
    3);
/** @brief Timer2 run enable. */
SBIT(TR2, 0xC8, 2);
/** @brief Timer2 clock source selection: 0=timer base internal clock, 1=external edge counter base T2 falling edge. */
SBIT(
    C_T2, 0xC8,
    1);
/** @brief Timer2 function selection (force 0 if RCLK=1 or TCLK=1): 0=timer and auto reload if count overflow or T2EX edge, 1=capture by T2EX edge. */
SBIT(
    CP_RL2, 0xC8,
    0);
/** @brief Timer 2 mode and timer 0/1/2 clock mode. */
SFR(T2MOD, 0xC9);
/** @brief Fastest internal clock mode for timer 0/1/2 under faster clock mode: 0=use divided clock, 1=use original Fsys as clock without dividing. */
#define bTMR_CLK                                                               \
    0x80
#define bT2_CLK                                                                \
    0x40 /* timer2 internal clock frequency selection:
             0=standard clock, Fsys/12 for timer mode, Fsys/4 for UART0 clock mode,
             1=faster clock, Fsys/4 @bTMR_CLK=0 or Fsys @bTMR_CLK=1 for timer mode,
               Fsys/2 @bTMR_CLK=0 or Fsys @bTMR_CLK=1 for UART0 clock mode */
/** @brief Timer1 internal clock frequency selection: 0=standard clock, Fsys/12, 1=faster clock, Fsys/4 if bTMR_CLK=0 or Fsys if bTMR_CLK=1. */
#define bT1_CLK                                                                \
    0x20
/** @brief Timer0 internal clock frequency selection: 0=standard clock, Fsys/12, 1=faster clock, Fsys/4 if bTMR_CLK=0 or Fsys if bTMR_CLK=1. */
#define bT0_CLK                                                                \
    0x10
/**
 * @brief Timer2 capture mode high bit.
 *
 * Capture point selection (bT2_CAP_M1 & bT2_CAP_M0):
 * - x0: falling edge to falling edge
 * - 01: any edge (level change)
 * - 11: rising edge to rising edge
 */
#define bT2_CAP_M1 0x08
/** @brief Timer2 capture mode low bit. */
#define bT2_CAP_M0 0x04
/** @brief Enable timer2 generated clock output: 0=disable output, 1=enable clock output at T2 pin, frequency = TF2/2. */
#define T2OE                                                                   \
    0x02
/** @brief Enable T2 trigger function for capture 1 of timer2 if RCLK=0 & TCLK=0 & CP_RL2=1 & C_T2=0 & T2OE=0. */
#define bT2_CAP1_EN                                                            \
    0x01
/** @brief Reload & capture value, little-endian. */
SFR16(RCAP2, 0xCA);
/** @brief Low byte of reload & capture value. */
SFR(RCAP2L, 0xCA);
/** @brief High byte of reload & capture value. */
SFR(RCAP2H, 0xCB);
/** @brief Counter, little-endian. */
SFR16(T2COUNT, 0xCC);
/** @brief Low byte of timer 2 count. */
SFR(TL2, 0xCC);
/** @brief High byte of timer 2 count. */
SFR(TH2, 0xCD);
/** @brief ReadOnly: capture 1 value for timer2. */
SFR16(T2CAP1, 0xCE);
/** @brief ReadOnly: capture 1 value low byte for timer2. */
SFR(T2CAP1L, 0xCE);
/** @brief ReadOnly: capture 1 value high byte for timer2. */
SFR(T2CAP1H, 0xCF);

/*  PWM1/2 Registers  */
/** @brief PWM data for PWM2. */
SFR(PWM_DATA2, 0x9B);
/** @brief PWM data for PWM1. */
SFR(PWM_DATA1, 0x9C);
/** @brief PWM 1/2 control. */
SFR(PWM_CTRL, 0x9D);
/** @brief Enable interrupt for PWM mode cycle end. */
#define bPWM_IE_END 0x80
/** @brief PWM2 output polarity: 0=default low and high action, 1=default high and low action. */
#define bPWM2_POLAR                                                            \
    0x40
/** @brief PWM1 output polarity: 0=default low and high action, 1=default high and low action. */
#define bPWM1_POLAR                                                            \
    0x20
/** @brief Interrupt flag for cycle end, write 1 to clear or write PWM_CYCLE or load new data to clear. */
#define bPWM_IF_END                                                            \
    0x10
/** @brief PWM2 output enable. */
#define bPWM2_OUT_EN 0x08
/** @brief PWM1 output enable. */
#define bPWM1_OUT_EN 0x04
/** @brief Force clear FIFO and count of PWM1/2. */
#define bPWM_CLR_ALL 0x02
/** @brief Clock divisor setting. */
SFR(PWM_CK_SE, 0x9E);

/*  SPI0/Master0/Slave Registers  */
/** @brief SPI 0 status. */
SFR(SPI0_STAT, 0xF8);
/** @brief ReadOnly: indicate first byte received status for SPI0. */
SBIT(S0_FST_ACT, 0xF8,
     7);
/** @brief Interrupt flag for slave mode FIFO overflow, direct bit address clear or write 1 to clear. */
SBIT(
    S0_IF_OV, 0xF8,
    6);
/** @brief Interrupt flag for first byte received, direct bit address clear or write 1 to clear. */
SBIT(
    S0_IF_FIRST, 0xF8,
    5);
/** @brief Interrupt flag for a byte data exchanged, direct bit address clear or write 1 to clear or accessing FIFO to clear if bS0_AUTO_IF=1. */
SBIT(
    S0_IF_BYTE, 0xF8,
    4);
/** @brief ReadOnly: SPI0 free status. */
SBIT(S0_FREE, 0xF8, 3);
/** @brief ReadOnly: tx FIFO count for SPI0. */
SBIT(S0_T_FIFO, 0xF8, 2);
/** @brief ReadOnly: rx FIFO count for SPI0. */
SBIT(S0_R_FIFO, 0xF8, 0);
/** @brief FIFO data port: reading for receiving, writing for transmittal. */
SFR(SPI0_DATA,
    0xF9);
/** @brief SPI 0 control. */
SFR(SPI0_CTRL, 0xFA);
/** @brief SPI0 MISO output enable. */
#define bS0_MISO_OE 0x80
/** @brief SPI0 MOSI output enable. */
#define bS0_MOSI_OE 0x40
/** @brief SPI0 SCK output enable. */
#define bS0_SCK_OE 0x20
/** @brief SPI0 data direction: 0=out(master_write), 1=in(master_read). */
#define bS0_DATA_DIR                                                           \
    0x10
/** @brief SPI0 master clock mode: 0=mode 0 with default low, 1=mode 3 with default high. */
#define bS0_MST_CLK                                                            \
    0x08
/** @brief Enable SPI0 two wire mode: 0=3 wire (SCK+MOSI+MISO), 1=2 wire (SCK+MISO). */
#define bS0_2_WIRE                                                             \
    0x04
/** @brief Force clear FIFO and count of SPI0. */
#define bS0_CLR_ALL 0x02
/** @brief Enable FIFO accessing to auto clear S0_IF_BYTE interrupt flag. */
#define bS0_AUTO_IF                                                            \
    0x01
/** @brief Clock divisor setting. */
SFR(SPI0_CK_SE, 0xFB);
//sfr SPI0_S_PRE      = 0xFB;         // preset value for SPI slave
#define SPI0_S_PRE SPI0_CK_SE
/** @brief SPI 0 setup. */
SFR(SPI0_SETUP, 0xFC);
/** @brief SPI0 slave mode: 0=master, 1=slave. */
#define bS0_MODE_SLV 0x80
/** @brief Enable interrupt for slave mode FIFO overflow. */
#define bS0_IE_FIFO_OV 0x40
/** @brief Enable interrupt for first byte received for SPI0 slave mode. */
#define bS0_IE_FIRST                                                           \
    0x20
/** @brief Enable interrupt for a byte received. */
#define bS0_IE_BYTE 0x10
/** @brief SPI0 bit data order: 0=MSB first, 1=LSB first. */
#define bS0_BIT_ORDER 0x08
/** @brief ReadOnly: SPI0 slave mode chip selected status: 0=unselected, 1=selected. */
#define bS0_SLV_SELT                                                           \
    0x02
/** @brief ReadOnly: SPI0 slave mode data pre-loading status just after chip-selection. */
#define bS0_SLV_PRELOAD                                                        \
    0x01

/*  UART1 Registers  */
/** @brief UART1 control (serial port control). */
SFR(SCON1, 0xC0);
/** @brief UART1 mode, selection data bit: 0=8 bits data, 1=9 bits data. */
SBIT(U1SM0, 0xC0,
     7);
/** @brief UART1 2X baud rate selection: 0=slow(Fsys/32/(256-SBAUD1)), 1=fast(Fsys/16/(256-SBAUD1)). */
SBIT(
    U1SMOD, 0xC0,
    5);
/** @brief Enable UART1 receiving. */
SBIT(U1REN, 0xC0, 4);
/** @brief The 9th transmitted data bit in 9 bits data mode. */
SBIT(U1TB8, 0xC0, 3);
/** @brief 9th data bit received in 9 bits data mode, or stop bit received for 8 bits data mode. */
SBIT(
    U1RB8, 0xC0,
    2);
/** @brief Transmit interrupt flag, set by hardware after completion of a serial transmittal, need software clear. */
SBIT(
    U1TI, 0xC0,
    1);
/** @brief Receive interrupt flag, set by hardware after completion of a serial receiving, need software clear. */
SBIT(
    U1RI, 0xC0,
    0);
/** @brief UART1 data buffer: reading for receiving, writing for transmittal. */
SFR(SBUF1,
    0xC1);
/** @brief UART1 baud rate setting. */
SFR(SBAUD1, 0xC2);

/*  ADC and comparator Registers  */
/**
 * @brief ADC control.
 *
 * IN+ channel selection (ADC_CHAN1 & ADC_CHAN0):
 * - 00: AIN0 (P1.1)
 * - 01: AIN1 (P1.4)
 * - 10: AIN2 (P1.5)
 * - 11: AIN3 (P3.2)
 */
SFR(ADC_CTRL, 0x80);
/** @brief ReadOnly: comparator result input. */
SBIT(CMPO, 0x80, 7);
/** @brief Flag for comparator result changed, direct bit address clear. */
SBIT(CMP_IF, 0x80,
     6);
/** @brief Interrupt flag for ADC finished, direct bit address clear. */
SBIT(ADC_IF, 0x80,
     5);
/** @brief Set 1 to start ADC, auto cleared when ADC finished. */
SBIT(ADC_START, 0x80, 4);
/** @brief Comparator IN- input channel selection: 0=AIN1, 1=AIN3. */
SBIT(CMP_CHAN, 0x80,
     3);
/** @brief ADC/comparator IN+ channel selection high bit. */
SBIT(ADC_CHAN1, 0x80, 1);
/** @brief ADC/comparator IN+ channel selection low bit. */
SBIT(ADC_CHAN0, 0x80, 0);
/** @brief ADC config. */
SFR(ADC_CFG, 0x9A);
/** @brief Control ADC power: 0=shut down ADC, 1=enable power for ADC. */
#define bADC_EN                                                                \
    0x08
/** @brief Control comparator power: 0=shut down comparator, 1=enable power for comparator. */
#define bCMP_EN                                                                \
    0x04
/** @brief ADC clock frequency selection: 0=slow clock, 384 Fosc cycles for each ADC, 1=fast clock, 96 Fosc cycles for each ADC. */
#define bADC_CLK                                                               \
    0x01
/** @brief ReadOnly: ADC data. */
SFR(ADC_DATA, 0x9F);

/*  Touch-key timer Registers  */
/**
 * @brief Touch-key control.
 *
 * Channel selection (bTKC_CHAN2:bTKC_CHAN0):
 * - 000: disable
 * - 001: TIN0 (P1.0)
 * - 010: TIN1 (P1.1)
 * - 011: TIN2 (P1.4)
 * - 100: TIN3 (P1.5)
 * - 101: TIN4 (P1.6)
 * - 110: TIN5 (P1.7)
 * - 111: enable, but no channel connected
 */
SFR(TKEY_CTRL, 0xC3);
/** @brief ReadOnly: interrupt flag for touch-key timer, cleared by writing touch-key control or auto cleared when start touch-key checking. */
#define bTKC_IF                                                                \
    0x80
/** @brief Touch-key timer cycle selection: 0=1mS, 1=2mS. */
#define bTKC_2MS 0x10
/** @brief Touch-key channel selection high bit. */
#define bTKC_CHAN2 0x04
/** @brief Touch-key channel selection middle bit. */
#define bTKC_CHAN1 0x02
/** @brief Touch-key channel selection low bit. */
#define bTKC_CHAN0 0x01
/** @brief ReadOnly: touch-key data, little-endian. */
SFR16(TKEY_DAT, 0xC4);
/** @brief ReadOnly: low byte of touch-key data. */
SFR(TKEY_DATL, 0xC4);
/** @brief ReadOnly: high byte of touch-key data. */
SFR(TKEY_DATH, 0xC5);
/** @brief ReadOnly: indicate control changed, current data maybe invalid. */
#define bTKD_CHG                                                               \
    0x80

/*  USB/Host/Device Registers  */
/** @brief USB type-C control. */
SFR(USB_C_CTRL, 0x91);
/** @brief USB VBUS2 10K pulldown resistance: 0=disable, 1=enable pullup. */
#define bVBUS2_PD_EN                                                           \
    0x80
/** @brief USB CC2 5.1K pulldown resistance: 0=disable, 1=enable pulldown. */
#define bUCC2_PD_EN                                                            \
    0x40
/** @brief USB CC2 pullup resistance control high bit. */
#define bUCC2_PU1_EN 0x20
/** @brief USB CC2 pullup resistance control low bit. */
#define bUCC2_PU0_EN 0x10
/** @brief USB VBUS1 10K pulldown resistance: 0=disable, 1=enable pullup. */
#define bVBUS1_PD_EN                                                           \
    0x08
/** @brief USB CC1 5.1K pulldown resistance: 0=disable, 1=enable pulldown. */
#define bUCC1_PD_EN                                                            \
    0x04
/** @brief USB CC1 pullup resistance control high bit. */
#define bUCC1_PU1_EN 0x02
/**
 * @brief USB CC1 pullup resistance control low bit.
 *
 * Pull-up selection (bUCC1/2_PU1_EN & bUCC1/2_PU0_EN):
 * - 00: disable pull-up
 * - 01: 56K pull-up (default USB power)
 * - 10: 22K pull-up (1.5A USB power)
 * - 11: 10K pull-up (3A USB power)
 */
#define bUCC1_PU0_EN 0x01
/** @brief USB device physical port control. */
SFR(UDEV_CTRL, 0xD1);
/** @brief Disable USB UDP/UDM pulldown resistance: 0=enable pulldown, 1=disable. */
#define bUD_PD_DIS                                                             \
    0x80
/** @brief ReadOnly: indicate current UDP pin level. */
#define bUD_DP_PIN 0x20
/** @brief ReadOnly: indicate current UDM pin level. */
#define bUD_DM_PIN 0x10
/** @brief Enable USB physical port low speed: 0=full speed, 1=low speed. */
#define bUD_LOW_SPEED                                                          \
    0x04
/** @brief General purpose bit. */
#define bUD_GP_BIT 0x02
/** @brief Enable USB physical port I/O: 0=disable, 1=enable. */
#define bUD_PORT_EN 0x01
#if defined(CHIP_CH554)
//sfr UHOST_CTRL      = 0xD1;         // USB host physical port control
#define UHOST_CTRL UDEV_CTRL
/** @brief Disable USB UDP/UDM pulldown resistance: 0=enable pulldown, 1=disable. */
#define bUH_PD_DIS                                                             \
    0x80
/** @brief ReadOnly: indicate current UDP pin level. */
#define bUH_DP_PIN 0x20
/** @brief ReadOnly: indicate current UDM pin level. */
#define bUH_DM_PIN 0x10
/** @brief Enable USB port low speed: 0=full speed, 1=low speed. */
#define bUH_LOW_SPEED                                                          \
    0x04
/** @brief Control USB bus reset: 0=normal, 1=force bus reset. */
#define bUH_BUS_RESET 0x02
/** @brief Enable USB port: 0=disable, 1=enable port, automatic disabled if USB device detached. */
#define bUH_PORT_EN                                                            \
    0x01
#endif
/** @brief Endpoint 1 control. */
SFR(UEP1_CTRL, 0xD2);
/** @brief Expected data toggle flag of USB endpoint X receiving (OUT): 0=DATA0, 1=DATA1. */
#define bUEP_R_TOG                                                             \
    0x80
/** @brief Prepared data toggle flag of USB endpoint X transmittal (IN): 0=DATA0, 1=DATA1. */
#define bUEP_T_TOG                                                             \
    0x40
/** @brief Enable automatic toggle after successful transfer completion on endpoint 1/2/3: 0=manual toggle, 1=automatic toggle. */
#define bUEP_AUTO_TOG                                                          \
    0x10
/** @brief Handshake response type high bit for USB endpoint X receiving (OUT). */
#define bUEP_R_RES1                                                            \
    0x08
/** @brief Handshake response type low bit for USB endpoint X receiving (OUT). */
#define bUEP_R_RES0                                                            \
    0x04
/**
 * @brief Bit mask of handshake response type for USB endpoint X receiving (OUT).
 *
 * Response codes (bUEP_R_RES1 & bUEP_R_RES0):
 * - 00: ACK (ready)
 * - 01: no response (timeout, for isochronous)
 * - 10: NAK (busy)
 * - 11: STALL (error)
 */
#define MASK_UEP_R_RES                                                         \
    0x0C
#define UEP_R_RES_ACK 0x00
#define UEP_R_RES_TOUT 0x04
#define UEP_R_RES_NAK 0x08
#define UEP_R_RES_STALL 0x0C
/** @brief Handshake response type high bit for USB endpoint X transmittal (IN). */
#define bUEP_T_RES1                                                            \
    0x02
/** @brief Handshake response type low bit for USB endpoint X transmittal (IN). */
#define bUEP_T_RES0                                                            \
    0x01
/**
 * @brief Bit mask of handshake response type for USB endpoint X transmittal (IN).
 *
 * Response codes (bUEP_T_RES1 & bUEP_T_RES0):
 * - 00: DATA0/1 + expect ACK (ready)
 * - 01: DATA0/1 + no handshake (timeout, for isochronous)
 * - 10: NAK (busy)
 * - 11: STALL (error)
 */
#define MASK_UEP_T_RES                                                         \
    0x03
#define UEP_T_RES_ACK 0x00
#define UEP_T_RES_TOUT 0x01
#define UEP_T_RES_NAK 0x02
#define UEP_T_RES_STALL 0x03
/** @brief Endpoint 1 transmittal length. */
SFR(UEP1_T_LEN, 0xD3);
/** @brief Endpoint 2 control. */
SFR(UEP2_CTRL, 0xD4);
/** @brief Endpoint 2 transmittal length. */
SFR(UEP2_T_LEN, 0xD5);
/** @brief Endpoint 3 control. */
SFR(UEP3_CTRL, 0xD6);
/** @brief Endpoint 3 transmittal length. */
SFR(UEP3_T_LEN, 0xD7);
/** @brief USB interrupt flag. */
SFR(USB_INT_FG, 0xD8);
/** @brief ReadOnly: indicate current USB transfer is NAK received. */
SBIT(U_IS_NAK, 0xD8,
     7);
/** @brief ReadOnly: indicate current USB transfer toggle is OK. */
SBIT(U_TOG_OK, 0xD8, 6);
/** @brief ReadOnly: indicate USB SIE free status. */
SBIT(U_SIE_FREE, 0xD8, 5);
/** @brief FIFO overflow interrupt flag for USB, direct bit address clear or write 1 to clear. */
SBIT(
    UIF_FIFO_OV, 0xD8,
    4);
#if defined(CHIP_CH554)
/** @brief Host SOF timer interrupt flag for USB host, direct bit address clear or write 1 to clear. */
SBIT(
    UIF_HST_SOF, 0xD8,
    3);
#endif
/** @brief USB suspend or resume event interrupt flag, direct bit address clear or write 1 to clear. */
SBIT(
    UIF_SUSPEND, 0xD8,
    2);
/** @brief USB transfer completion interrupt flag, direct bit address clear or write 1 to clear. */
SBIT(
    UIF_TRANSFER, 0xD8,
    1);
#if defined(CHIP_CH554)
/** @brief Device detected event interrupt flag for USB host mode, direct bit address clear or write 1 to clear. */
SBIT(
    UIF_DETECT, 0xD8,
    0);
#endif
/** @brief Bus reset event interrupt flag for USB device mode, direct bit address clear or write 1 to clear. */
SBIT(
    UIF_BUS_RST, 0xD8,
    0);
/** @brief ReadOnly: USB interrupt status. */
SFR(USB_INT_ST, 0xD9);
/** @brief ReadOnly: indicate current USB transfer is NAK received for USB device mode. */
#define bUIS_IS_NAK                                                            \
    0x80
/** @brief ReadOnly: indicate current USB transfer toggle is OK. */
#define bUIS_TOG_OK 0x40
/** @brief ReadOnly: current token PID code bit 1 received for USB device mode. */
#define bUIS_TOKEN1                                                            \
    0x20
/** @brief ReadOnly: current token PID code bit 0 received for USB device mode. */
#define bUIS_TOKEN0                                                            \
    0x10
/**
 * @brief (ReadOnly) Bit mask of current token PID code received for USB device mode.
 *
 * Token codes (bUIS_TOKEN1 & bUIS_TOKEN0):
 * - 00: OUT
 * - 01: SOF
 * - 10: IN
 * - 11: SETUP
 */
#define MASK_UIS_TOKEN                                                         \
    0x30
#define UIS_TOKEN_OUT 0x00
#define UIS_TOKEN_SOF 0x10
#define UIS_TOKEN_IN 0x20
#define UIS_TOKEN_SETUP 0x30
/** @brief ReadOnly: bit mask of current transfer endpoint number for USB device mode. */
#define MASK_UIS_ENDP                                                          \
    0x0F
#if defined(CHIP_CH554)
/** @brief ReadOnly: bit mask of current transfer handshake response for USB host mode: 0000=no response, time out from device, others=handshake response PID received. */
#define MASK_UIS_H_RES                                                         \
    0x0F
#endif
/** @brief ReadOnly: USB miscellaneous status. */
SFR(USB_MIS_ST, 0xDA);
#if defined(CHIP_CH554)
/** @brief ReadOnly: indicate host SOF timer presage status. */
#define bUMS_SOF_PRES 0x80
/** @brief ReadOnly: indicate host SOF timer action status for USB host. */
#define bUMS_SOF_ACT                                                           \
    0x40
#endif
/** @brief ReadOnly: indicate USB SIE free status. */
#define bUMS_SIE_FREE 0x20
/** @brief ReadOnly: indicate USB receiving FIFO ready status (not empty). */
#define bUMS_R_FIFO_RDY                                                        \
    0x10
/** @brief ReadOnly: indicate USB bus reset status. */
#define bUMS_BUS_RESET 0x08
/** @brief ReadOnly: indicate USB suspend status. */
#define bUMS_SUSPEND 0x04
#if defined(CHIP_CH554)
/** @brief ReadOnly: indicate UDM level saved at device attached to USB host. */
#define bUMS_DM_LEVEL                                                          \
    0x02
/** @brief ReadOnly: indicate device attached status on USB host. */
#define bUMS_DEV_ATTACH                                                        \
    0x01
#endif
/** @brief ReadOnly: USB receiving length. */
SFR(USB_RX_LEN, 0xDB);
/** @brief Endpoint 0 control. */
SFR(UEP0_CTRL, 0xDC);
/** @brief Endpoint 0 transmittal length. */
SFR(UEP0_T_LEN, 0xDD);
/** @brief Endpoint 4 control. */
SFR(UEP4_CTRL, 0xDE);
/** @brief Endpoint 4 transmittal length. */
SFR(UEP4_T_LEN, 0xDF);
/** @brief USB interrupt enable. */
SFR(USB_INT_EN, 0xE1);
/** @brief Enable interrupt for SOF received for USB device mode. */
#define bUIE_DEV_SOF                                                           \
    0x80
/** @brief Enable interrupt for NAK responded for USB device mode. */
#define bUIE_DEV_NAK                                                           \
    0x40
/** @brief Enable interrupt for FIFO overflow. */
#define bUIE_FIFO_OV 0x10
#if defined(CHIP_CH554)
/** @brief Enable interrupt for host SOF timer action for USB host mode. */
#define bUIE_HST_SOF                                                           \
    0x08
#endif
/** @brief Enable interrupt for USB suspend or resume event. */
#define bUIE_SUSPEND 0x04
/** @brief Enable interrupt for USB transfer completion. */
#define bUIE_TRANSFER 0x02
#if defined(CHIP_CH554)
/** @brief Enable interrupt for USB device detected event for USB host mode. */
#define bUIE_DETECT                                                            \
    0x01
#endif
/** @brief Enable interrupt for USB bus reset event for USB device mode. */
#define bUIE_BUS_RST                                                           \
    0x01
/** @brief USB base control. */
SFR(USB_CTRL, 0xE2);
/** @brief Enable USB host mode: 0=device mode, 1=host mode. */
#define bUC_HOST_MODE 0x80
/** @brief Enable USB low speed: 0=full speed, 1=low speed. */
#define bUC_LOW_SPEED 0x40
/** @brief USB device enable and internal pullup resistance enable. */
#define bUC_DEV_PU_EN                                                          \
    0x20
/** @brief USB system control high bit. */
#define bUC_SYS_CTRL1 0x20
/** @brief USB system control low bit. */
#define bUC_SYS_CTRL0 0x10
/**
 * @brief Bit mask of USB system control (bUC_HOST_MODE & bUC_SYS_CTRL).
 *
 * Device mode (bUC_HOST_MODE = 0):
 * - 000: disable USB device, no pull-up
 * - 001: enable USB device, external pull-up needed
 * - 01x: enable USB device, internal pull-up on
 *
 * @note Host mode (bUC_HOST_MODE = 1, CH554 only):
 * - 100: enable host, normal
 * - 101: enable host, force SE0 on D+/D-
 * - 110: enable host, force J state
 * - 111: enable host, force resume/K state
 */
#define MASK_UC_SYS_CTRL 0x30
/** @brief Enable automatic responding busy for device mode or automatic pause for host mode during interrupt flag UIF_TRANSFER valid. */
#define bUC_INT_BUSY                                                           \
    0x08
/** @brief Force reset USB SIE, need software clear. */
#define bUC_RESET_SIE 0x04
/** @brief Force clear FIFO and count of USB. */
#define bUC_CLR_ALL 0x02
/** @brief DMA enable and DMA interrupt enable for USB. */
#define bUC_DMA_EN 0x01
/** @brief USB device address, lower 7 bits for USB device address. */
SFR(USB_DEV_AD,
    0xE3);
/** @brief General purpose bit. */
#define bUDA_GP_BIT 0x80
/** @brief Bit mask for USB device address. */
#define MASK_USB_ADDR 0x7F
/** @brief Endpoint 2 buffer start address, little-endian. */
SFR16(UEP2_DMA, 0xE4);
/** @brief Endpoint 2 buffer start address low byte. */
SFR(UEP2_DMA_L, 0xE4);
/** @brief Endpoint 2 buffer start address high byte. */
SFR(UEP2_DMA_H, 0xE5);
/** @brief Endpoint 3 buffer start address, little-endian. */
SFR16(UEP3_DMA, 0xE6);
/** @brief Endpoint 3 buffer start address low byte. */
SFR(UEP3_DMA_L, 0xE6);
/** @brief Endpoint 3 buffer start address high byte. */
SFR(UEP3_DMA_H, 0xE7);
/**
 * @brief Endpoint 4/1 mode.
 *
 * EP1/2/3 buffer selection (bUEPn_RX_EN & bUEPn_TX_EN & bUEPn_BUF_MOD):
 * - 000: disable buffer
 * - 100: 64 B, RX (OUT)
 * - 101: dual 64 B, RX (OUT), 128 B total
 * - 010: 64 B, TX (IN)
 * - 011: dual 64 B, TX (IN), 128 B total
 * - 110: 64 B RX + 64 B TX, 128 B total
 * - 111: dual 64 B RX + dual 64 B TX, 256 B total
 */
SFR(UEP4_1_MOD, 0xEA);
/** @brief Enable USB endpoint 1 receiving (OUT). */
#define bUEP1_RX_EN 0x80
/** @brief Enable USB endpoint 1 transmittal (IN). */
#define bUEP1_TX_EN 0x40
/** @brief Buffer mode of USB endpoint 1. */
#define bUEP1_BUF_MOD 0x10
/** @brief Enable USB endpoint 4 receiving (OUT). */
#define bUEP4_RX_EN 0x08
/**
 * @brief Enable USB endpoint 4 transmittal (IN).
 *
 * EP4 buffer selection (bUEP4_RX_EN & bUEP4_TX_EN, base = UEP0_DMA):
 * - 00: EP0 only (64 B)
 * - 10: EP0 + EP4 RX (OUT), 128 B total
 * - 01: EP0 + EP4 TX (IN), 128 B total
 * - 11: EP0 + EP4 RX + EP4 TX, 192 B total
 */
#define bUEP4_TX_EN 0x04
/** @brief Endpoint 2/3 mode. */
SFR(UEP2_3_MOD, 0xEB);
/** @brief Enable USB endpoint 3 receiving (OUT). */
#define bUEP3_RX_EN 0x80
/** @brief Enable USB endpoint 3 transmittal (IN). */
#define bUEP3_TX_EN 0x40
/** @brief Buffer mode of USB endpoint 3. */
#define bUEP3_BUF_MOD 0x10
/** @brief Enable USB endpoint 2 receiving (OUT). */
#define bUEP2_RX_EN 0x08
/** @brief Enable USB endpoint 2 transmittal (IN). */
#define bUEP2_TX_EN 0x04
/** @brief Buffer mode of USB endpoint 2. */
#define bUEP2_BUF_MOD 0x01
/** @brief Endpoint 0 buffer start address, little-endian. */
SFR16(UEP0_DMA, 0xEC);
/** @brief Endpoint 0 buffer start address low byte. */
SFR(UEP0_DMA_L, 0xEC);
/** @brief Endpoint 0 buffer start address high byte. */
SFR(UEP0_DMA_H, 0xED);
/** @brief Endpoint 1 buffer start address, little-endian. */
SFR16(UEP1_DMA, 0xEE);
/** @brief Endpoint 1 buffer start address low byte. */
SFR(UEP1_DMA_L, 0xEE);
/** @brief Endpoint 1 buffer start address high byte. */
SFR(UEP1_DMA_H, 0xEF);
#if defined(CHIP_CH554)
//sfr UH_SETUP        = 0xD2;         // host aux setup
#define UH_SETUP UEP1_CTRL
/** @brief USB host PRE PID enable for low speed device via hub. */
#define bUH_PRE_PID_EN                                                         \
    0x80
/** @brief USB host automatic SOF enable. */
#define bUH_SOF_EN 0x40
//sfr UH_RX_CTRL      = 0xD4;         // host receiver endpoint control
#define UH_RX_CTRL UEP2_CTRL
/** @brief Expected data toggle flag of host receiving (IN): 0=DATA0, 1=DATA1. */
#define bUH_R_TOG                                                              \
    0x80
/** @brief Enable automatic toggle after successful transfer completion: 0=manual toggle, 1=automatic toggle. */
#define bUH_R_AUTO_TOG                                                         \
    0x10
/** @brief Prepared handshake response type for host receiving (IN): 0=ACK (ready), 1=no response, time out to device, for isochronous transactions. */
#define bUH_R_RES                                                              \
    0x04
//sfr UH_EP_PID       = 0xD5;         // host endpoint and token PID, lower 4 bits for endpoint number, upper 4 bits for token PID
#define UH_EP_PID UEP2_T_LEN
/** @brief Bit mask of token PID for USB host transfer. */
#define MASK_UH_TOKEN 0xF0
/** @brief Bit mask of endpoint number for USB host transfer. */
#define MASK_UH_ENDP 0x0F
//sfr UH_TX_CTRL      = 0xD6;         // host transmittal endpoint control
#define UH_TX_CTRL UEP3_CTRL
/** @brief Prepared data toggle flag of host transmittal (SETUP/OUT): 0=DATA0, 1=DATA1. */
#define bUH_T_TOG                                                              \
    0x40
/** @brief Enable automatic toggle after successful transfer completion: 0=manual toggle, 1=automatic toggle. */
#define bUH_T_AUTO_TOG                                                         \
    0x10
/** @brief Expected handshake response type for host transmittal (SETUP/OUT): 0=ACK (ready), 1=no response, time out from device, for isochronous transactions. */
#define bUH_T_RES                                                              \
    0x01
//sfr UH_TX_LEN       = 0xD7;         // host transmittal endpoint transmittal length
#define UH_TX_LEN UEP3_T_LEN
//sfr UH_EP_MOD       = 0xEB;         // host endpoint mode
#define UH_EP_MOD UEP2_3_MOD
/** @brief Enable USB host OUT endpoint transmittal. */
#define bUH_EP_TX_EN 0x40
/**
 * @brief Buffer mode of USB host OUT endpoint.
 *
 * OUT buffer selection (bUH_EP_TX_EN & bUH_EP_TBUF_MOD, base = UH_TX_DMA):
 * - 00: disable
 * - 10: 64 B TX (OUT)
 * - 11: dual 64 B TX (OUT), toggle by bUH_T_TOG, 128 B total
 */
#define bUH_EP_TBUF_MOD 0x10
/** @brief Enable USB host IN endpoint receiving. */
#define bUH_EP_RX_EN 0x08
/**
 * @brief Buffer mode of USB host IN endpoint.
 *
 * IN buffer selection (bUH_EP_RX_EN & bUH_EP_RBUF_MOD, base = UH_RX_DMA):
 * - 00: disable
 * - 10: 64 B RX (IN)
 * - 11: dual 64 B RX (IN), toggle by bUH_R_TOG, 128 B total
 */
#define bUH_EP_RBUF_MOD 0x01
//sfr16 UH_RX_DMA     = 0xE4;         // host rx endpoint buffer start address, little-endian
#define UH_RX_DMA UEP2_DMA
//sfr UH_RX_DMA_L     = 0xE4;         // host rx endpoint buffer start address low byte
#define UH_RX_DMA_L UEP2_DMA_L
//sfr UH_RX_DMA_H     = 0xE5;         // host rx endpoint buffer start address high byte
#define UH_RX_DMA_H UEP2_DMA_H
//sfr16 UH_TX_DMA     = 0xE6;         // host tx endpoint buffer start address, little-endian
#define UH_TX_DMA UEP3_DMA
//sfr UH_TX_DMA_L     = 0xE6;         // host tx endpoint buffer start address low byte
#define UH_TX_DMA_L UEP3_DMA_L
//sfr UH_TX_DMA_H     = 0xE7;         // host tx endpoint buffer start address high byte
#define UH_TX_DMA_H UEP3_DMA_H
#endif

/*----- XDATA: xRAM ------------------------------------------*/

/** @brief Size of expanded xRAM, xdata SRAM embedded chip. */
#define XDATA_RAM_SIZE 0x0400

/*----- Reference Information --------------------------------------------*/
#if defined(CHIP_CH554)
/** @brief Chip ID for CH554. */
#define ID_CH55x 0x54
#elif defined(CHIP_CH552)
/** @brief Chip ID for CH552. */
#define ID_CH55x 0x52
#else
#error "Must define CHIP_CH552 or CHIP_CH554"
#endif

/* Interrupt routine address and interrupt number */
/** @brief Interrupt vector address for INT0. */
#define INT_ADDR_INT0 0x0003
/** @brief Interrupt vector address for timer0. */
#define INT_ADDR_TMR0 0x000B
/** @brief Interrupt vector address for INT1. */
#define INT_ADDR_INT1 0x0013
/** @brief Interrupt vector address for timer1. */
#define INT_ADDR_TMR1 0x001B
/** @brief Interrupt vector address for UART0. */
#define INT_ADDR_UART0 0x0023
/** @brief Interrupt vector address for timer2. */
#define INT_ADDR_TMR2 0x002B
/** @brief Interrupt vector address for SPI0. */
#define INT_ADDR_SPI0 0x0033
/** @brief Interrupt vector address for touch-key timer. */
#define INT_ADDR_TKEY 0x003B
/** @brief Interrupt vector address for USB. */
#define INT_ADDR_USB 0x0043
/** @brief Interrupt vector address for ADC. */
#define INT_ADDR_ADC 0x004B
/** @brief Interrupt vector address for UART1. */
#define INT_ADDR_UART1 0x0053
/** @brief Interrupt vector address for PWM1/2. */
#define INT_ADDR_PWMX 0x005B
/** @brief Interrupt vector address for GPIO. */
#define INT_ADDR_GPIO 0x0063
/** @brief Interrupt vector address for watch-dog timer. */
#define INT_ADDR_WDOG 0x006B
/** @brief Interrupt number for INT0. */
#define INT_NO_INT0 0
/** @brief Interrupt number for timer0. */
#define INT_NO_TMR0 1
/** @brief Interrupt number for INT1. */
#define INT_NO_INT1 2
/** @brief Interrupt number for timer1. */
#define INT_NO_TMR1 3
/** @brief Interrupt number for UART0. */
#define INT_NO_UART0 4
/** @brief Interrupt number for timer2. */
#define INT_NO_TMR2 5
/** @brief Interrupt number for SPI0. */
#define INT_NO_SPI0 6
/** @brief Interrupt number for touch-key timer. */
#define INT_NO_TKEY 7
/** @brief Interrupt number for USB. */
#define INT_NO_USB 8
/** @brief Interrupt number for ADC. */
#define INT_NO_ADC 9
/** @brief Interrupt number for UART1. */
#define INT_NO_UART1 10
/** @brief Interrupt number for PWM1/2. */
#define INT_NO_PWMX 11
/** @brief Interrupt number for GPIO. */
#define INT_NO_GPIO 12
/** @brief Interrupt number for watch-dog timer. */
#define INT_NO_WDOG 13

/* Special Program Space */
/** @brief Start address of Data-Flash. */
#define DATA_FLASH_ADDR 0xC000
/** @brief Start address of boot loader program. */
#define BOOT_LOAD_ADDR 0x3800
/** @brief Chip configuration information address. */
#define ROM_CFG_ADDR 0x3FF8
/** @brief Chip ID number highest byte (only low byte valid). */
#define ROM_CHIP_ID_HX                                                         \
    0x3FFA
/** @brief Chip ID number low word. */
#define ROM_CHIP_ID_LO 0x3FFC
/** @brief Chip ID number high word. */
#define ROM_CHIP_ID_HI 0x3FFE

/*
New Instruction:   MOVX @DPTR1,A
Instruction Code:  0xA5
Instruction Cycle: 1
Instruction Operation:
   step-1. write ACC @DPTR1 into xdata SRAM embedded chip
   step-2. increase DPTR1
ASM example:
       INC  XBUS_AUX
       MOV  DPTR,#TARGET_ADDR ;DPTR1
       DEC  XBUS_AUX
       MOV  DPTR,#SOURCE_ADDR ;DPTR0
       MOV  R7,#xxH
 LOOP: MOVX A,@DPTR ;DPTR0
       INC  DPTR    ;DPTR0, if need
       .DB  0xA5    ;MOVX @DPTR1,A & INC DPTR1
       DJNZ R7,LOOP
*/

#endif // __CH55X_H__
