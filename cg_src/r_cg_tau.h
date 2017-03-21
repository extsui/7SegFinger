/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2012, 2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_tau.h
* Version      : Code Generator for RL78/G10 V1.04.03.03 [07 Mar 2016]
* Device(s)    : R5F10Y47
* Tool-Chain   : CCRL
* Description  : This file implements device driver for TAU module.
* Creation Date: 2017/03/22
***********************************************************************************************************************/
#ifndef TAU_H
#define TAU_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    Peripheral Enable Register 0 (PER0)
*/
/* Control of timer array unit 0 input clock (TAU0EN) */
#define _00_TAU0_CLOCK_STOP                     (0x00U) /* stops supply of input clock */
#define _01_TAU0_CLOCK_SUPPLY                   (0x01U) /* supplies input clock */

/*
    Timer Clock Select Register m (TPSm)
*/
/* Operating mode and clear mode selection (PRSm03 - PRSm00) */
#define _00_TAU_CKM0_FCLK_0                     (0x00U) /* ckm0 - fCLK */
#define _01_TAU_CKM0_FCLK_1                     (0x01U) /* ckm0 - fCLK/2^1 */
#define _02_TAU_CKM0_FCLK_2                     (0x02U) /* ckm0 - fCLK/2^2 */
#define _03_TAU_CKM0_FCLK_3                     (0x03U) /* ckm0 - fCLK/2^3 */
#define _04_TAU_CKM0_FCLK_4                     (0x04U) /* ckm0 - fCLK/2^4 */
#define _05_TAU_CKM0_FCLK_5                     (0x05U) /* ckm0 - fCLK/2^5 */
#define _06_TAU_CKM0_FCLK_6                     (0x06U) /* ckm0 - fCLK/2^6 */
#define _07_TAU_CKM0_FCLK_7                     (0x07U) /* ckm0 - fCLK/2^7 */
#define _08_TAU_CKM0_FCLK_8                     (0x08U) /* ckm0 - fCLK/2^8 */
#define _09_TAU_CKM0_FCLK_9                     (0x09U) /* ckm0 - fCLK/2^9 */
#define _0A_TAU_CKM0_FCLK_10                    (0x0AU) /* ckm0 - fCLK/2^10 */
#define _0B_TAU_CKM0_FCLK_11                    (0x0BU) /* ckm0 - fCLK/2^11 */
#define _0C_TAU_CKM0_FCLK_12                    (0x0CU) /* ckm0 - fCLK/2^12 */
#define _0D_TAU_CKM0_FCLK_13                    (0x0DU) /* ckm0 - fCLK/2^13 */
#define _0E_TAU_CKM0_FCLK_14                    (0x0EU) /* ckm0 - fCLK/2^14 */
#define _0F_TAU_CKM0_FCLK_15                    (0x0FU) /* ckm0 - fCLK/2^15 */
/* Operating mode and clear mode selection (PRSm13 - PRSm10) */
#define _00_TAU_CKM1_FCLK_0                     (0x00U) /* ckm1 - fCLK */
#define _10_TAU_CKM1_FCLK_1                     (0x10U) /* ckm1 - fCLK/2^1 */
#define _20_TAU_CKM1_FCLK_2                     (0x20U) /* ckm1 - fCLK/2^2 */
#define _30_TAU_CKM1_FCLK_3                     (0x30U) /* ckm1 - fCLK/2^3 */
#define _40_TAU_CKM1_FCLK_4                     (0x40U) /* ckm1 - fCLK/2^4 */
#define _50_TAU_CKM1_FCLK_5                     (0x50U) /* ckm1 - fCLK/2^5 */
#define _60_TAU_CKM1_FCLK_6                     (0x60U) /* ckm1 - fCLK/2^6 */
#define _70_TAU_CKM1_FCLK_7                     (0x70U) /* ckm1 - fCLK/2^7 */
#define _80_TAU_CKM1_FCLK_8                     (0x80U) /* ckm1 - fCLK/2^8 */
#define _90_TAU_CKM1_FCLK_9                     (0x90U) /* ckm1 - fCLK/2^9 */
#define _A0_TAU_CKM1_FCLK_10                    (0xA0U) /* ckm1 - fCLK/2^10 */
#define _B0_TAU_CKM1_FCLK_11                    (0xB0U) /* ckm1 - fCLK/2^11 */
#define _C0_TAU_CKM1_FCLK_12                    (0xC0U) /* ckm1 - fCLK/2^12 */
#define _D0_TAU_CKM1_FCLK_13                    (0xD0U) /* ckm1 - fCLK/2^13 */
#define _E0_TAU_CKM1_FCLK_14                    (0xE0U) /* ckm1 - fCLK/2^14 */
#define _F0_TAU_CKM1_FCLK_15                    (0xF0U) /* ckm1 - fCLK/2^15 */

/*
    Timer Channel Mode Register mn (TMRmnH)
*/
/* Selection of macro clock (MCK) of channel n (CKSmn1H) */
#define _00_TAU_CLOCK_SELECT_CKM0               (0x00U) /* operation clock CK0 set by PRS register */ 
#define _80_TAU_CLOCK_SELECT_CKM1               (0x80U) /* operation clock CK1 set by PRS register */
/* Selection of count clock (CCK) of channel n (CCSmnH) */
#define _00_TAU_CLOCK_MODE_CKS                  (0x00U) /* macro clock MCK specified by CKSmn bit */  
#define _10_TAU_CLOCK_MODE_TIMN                 (0x10U) /* valid edge of input signal input from TImn pin */
/* Selection of slave/master of channel n (MASTERmnH) */
#define _00_TAU_COMBINATION_SLAVE               (0x00U) /* operates as slave channel */
#define _08_TAU_COMBINATION_MASTER              (0x08U) /* operates as master channel */
/* Operation explanation of channel 1 (SPLITH) */
#define _00_TAU_16BITS_MODE                     (0x00U) /* operates as 16 bits timer */  
#define _08_TAU_8BITS_MODE                      (0x08U) /* operates as 8 bits timer */
/* Setting of start trigger or capture trigger of channel n (STSmn2H - STSmn0H) */
#define _00_TAU_TRIGGER_SOFTWARE                (0x00U) /* only software trigger start is valid */
#define _01_TAU_TRIGGER_TIMN_VALID              (0x01U) /* TImn input edge is used as a start/capture trigger */
#define _02_TAU_TRIGGER_TIMN_BOTH               (0x02U) /* TImn input edges are used as a start/capture trigger */
#define _04_TAU_TRIGGER_MASTER_INT              (0x04U) /* interrupt signal of the master channel is used */

/*
    Timer Channel Mode Register mn (TMRmnL)
*/
/* Selection of TImn pin input valid edge (CISmn1L - CISmn0L) */
#define _00_TAU_TIMN_EDGE_FALLING               (0x00U) /* falling edge */
#define _40_TAU_TIMN_EDGE_RISING                (0x40U) /* rising edge */
#define _80_TAU_TIMN_EDGE_BOTH_LOW              (0x80U) /* both edges (when low-level width is measured) */
#define _C0_TAU_TIMN_EDGE_BOTH_HIGH             (0xC0U) /* both edges (when high-level width is measured) */
/* Operation mode of channel n (MDmn3L - MDmn0L) */
#define _00_TAU_MODE_INTERVAL_TIMER             (0x00U) /* interval timer mode */
#define _04_TAU_MODE_CAPTURE                    (0x04U) /* capture mode */
#define _06_TAU_MODE_EVENT_COUNT                (0x06U) /* event counter mode */
#define _08_TAU_MODE_ONE_COUNT                  (0x08U) /* one count mode */
#define _0C_TAU_MODE_HIGHLOW_MEASURE            (0x0CU) /* high-/low-level width measurement mode */
#define _01_TAU_MODE_PWM_MASTER                 (0x01U) /* PWM function (master channel) mode */
#define _09_TAU_MODE_PWM_SLAVE                  (0x09U) /* PWM function (slave channel) mode */
#define _08_TAU_MODE_ONESHOT                    (0x08U) /* one-shot pulse output mode */
/* Setting of starting counting and interrupt (MDmn0L) */
#define _00_TAU_START_INT_UNUSED                (0x00U) /* interrupt is not generated when counting is started */
#define _01_TAU_START_INT_USED                  (0x01U) /* interrupt is generated when counting is started */

/*
    Timer Status Register mn (TSRmn)
*/
/* Counter overflow status of channel n (OVF) */
#define _00_TAU_OVERFLOW_NOT_OCCURS             (0x00U) /* overflow does not occur */
#define _01_TAU_OVERFLOW_OCCURS                 (0x01U) /* overflow occurs */

/*
    Timer Channel Enable Status Register m (TEm)
*/
/* Indication of operation enable/stop status of channel 0 (TEm0) */
#define _00_TAU_CH0_OPERATION_STOP              (0x00U) /* operation is stopped */
#define _01_TAU_CH0_OPERATION_ENABLE            (0x01U) /* operation is enabled */
/* Indication of operation enable/stop status of channel 1 (TEm1) */
#define _00_TAU_CH1_OPERATION_STOP              (0x00U) /* operation is stopped */
#define _02_TAU_CH1_OPERATION_ENABLE            (0x02U) /* operation is enabled */
/* Indication of operation enable/stop status of channel 2 (TEm2) */
#define _00_TAU_CH2_OPERATION_STOP              (0x00U) /* operation is stopped */
#define _04_TAU_CH2_OPERATION_ENABLE            (0x04U) /* operation is enabled */
/* Indication of operation enable/stop status of channel 3 (TEm3) */
#define _00_TAU_CH3_OPERATION_STOP              (0x00U) /* operation is stopped */
#define _08_TAU_CH3_OPERATION_ENABLE            (0x08U) /* operation is enabled */

/*
    Timer Channel Enable Status Register m (TEHm)
*/
/* Indication of operation enable/stop status of channel 1 higher 8 bits (TEHm1) */
#define _00_TAU_CH1_H8_OPERATION_STOP           (0x00U) /* operation is stopped */
#define _02_TAU_CH1_H8_OPERATION_ENABLE         (0x02U) /* operation is enabled */
/* Indication of operation enable/stop status of channel 3 higher 8 bits (TEHm3) */
#define _00_TAU_CH3_H8_OPERATION_STOP           (0x00U) /* operation is stopped */
#define _08_TAU_CH3_H8_OPERATION_ENABLE         (0x08U) /* operation is enabled */

/*
    Timer Channel Start Register m (TSm)
*/
/* Operation enable (start) trigger of channel 0 (TSm0) */
#define _00_TAU_CH0_START_TRG_OFF               (0x00U) /* no trigger operation */
#define _01_TAU_CH0_START_TRG_ON                (0x01U) /* operation is enabled (start trigger is generated) */
/* Operation enable (start) trigger of channel 1 (TSm1) */
#define _00_TAU_CH1_START_TRG_OFF               (0x00U) /* no trigger operation */
#define _02_TAU_CH1_START_TRG_ON                (0x02U) /* operation is enabled (start trigger is generated) */
/* Operation enable (start) trigger of channel 2 (TSm2) */
#define _00_TAU_CH2_START_TRG_OFF               (0x00U) /* no trigger operation */
#define _04_TAU_CH2_START_TRG_ON                (0x04U) /* operation is enabled (start trigger is generated) */
/* Operation enable (start) trigger of channel 3 (TSm3) */
#define _00_TAU_CH3_START_TRG_OFF               (0x00U) /* no trigger operation */
#define _08_TAU_CH3_START_TRG_ON                (0x08U) /* operation is enabled (start trigger is generated) */

/*
    Timer Channel Start Register m (TSHm)
*/
/* Operation enable (start) trigger of channel 1 higher 8 bits (TSHm1) */
#define _00_TAU_CH1_H8_START_TRG_OFF            (0x00U) /* no trigger operation */
#define _02_TAU_CH1_H8_START_TRG_ON             (0x02U) /* operation is enabled (start trigger is generated) */
/* Operation enable (start) trigger of channel 3 higher 8 bits (TSHm3) */
#define _00_TAU_CH3_H8_START_TRG_OFF            (0x00U) /* no trigger operation */
#define _08_TAU_CH3_H8_START_TRG_ON             (0x08U) /* operation is enabled (start trigger is generated) */

/*
    Timer Channel Stop Register m (TTm)
*/
/* Operation stop trigger of channel 0 (TTm0) */
#define _00_TAU_CH0_STOP_TRG_OFF                (0x00U) /* no trigger operation */
#define _01_TAU_CH0_STOP_TRG_ON                 (0x01U) /* operation is stopped (stop trigger is generated) */
/* Operation stop trigger of channel 1 (TTm1) */
#define _00_TAU_CH1_STOP_TRG_OFF                (0x00U) /* no trigger operation */
#define _02_TAU_CH1_STOP_TRG_ON                 (0x02U) /* operation is stopped (stop trigger is generated) */
/* Operation stop trigger of channel 2 (TTm2) */
#define _00_TAU_CH2_STOP_TRG_OFF                (0x00U) /* no trigger operation */
#define _04_TAU_CH2_STOP_TRG_ON                 (0x04U) /* operation is stopped (stop trigger is generated) */
/* Operation stop trigger of channel 3 (TTm3) */
#define _00_TAU_CH3_STOP_TRG_OFF                (0x00U) /* no trigger operation */
#define _08_TAU_CH3_STOP_TRG_ON                 (0x08U) /* operation is stopped (stop trigger is generated) */

/*
    Timer Channel Stop Register m (TTHm)
*/
/* Operation stop trigger of channel 1 higher 8 bits (TTHm1) */
#define _00_TAU_CH1_H8_STOP_TRG_OFF             (0x00U) /* no trigger operation */
#define _02_TAU_CH1_H8_STOP_TRG_ON              (0x02U) /* operation is stopped (stop trigger is generated) */
/* Operation stop trigger of channel 3 higher 8 bits (TTHm3) */
#define _00_TAU_CH3_H8_STOP_TRG_OFF             (0x00U) /* no trigger operation */
#define _08_TAU_CH3_H8_STOP_TRG_ON              (0x08U) /* operation is stopped (stop trigger is generated) */

/*
    Timer Output Enable Register m (TOEm)
*/
/* Timer output enable/disable of channel 0 (TOEm0) */
#define _01_TAU_CH0_OUTPUT_ENABLE               (0x01U) /* the TOm0 operation enabled by count operation */
#define _00_TAU_CH0_OUTPUT_DISABLE              (0x00U) /* the TOm0 operation stopped by count operation */
/* Timer output enable/disable of channel 1 (TOEm1) */
#define _02_TAU_CH1_OUTPUT_ENABLE               (0x02U) /* the TOm1 operation enabled by count operation */
#define _00_TAU_CH1_OUTPUT_DISABLE              (0x00U) /* the TOm1 operation stopped by count operation */
/* Timer output enable/disable of channel 2 (TOEm2) */
#define _04_TAU_CH2_OUTPUT_ENABLE               (0x04U) /* the TOm2 operation enabled by count operation */
#define _00_TAU_CH2_OUTPUT_DISABLE              (0x00U) /* the TOm2 operation stopped by count operation */
/* Timer output enable/disable of channel 3 (TOEm3) */
#define _08_TAU_CH3_OUTPUT_ENABLE               (0x08U) /* the TOm3 operation enabled by count operation */
#define _00_TAU_CH3_OUTPUT_DISABLE              (0x00U) /* the TOm3 operation stopped by count operation */

/*
    Timer Output Register m (TOm)
*/
/* Timer output of channel 0 (TOm0) */
#define _00_TAU_CH0_OUTPUT_VALUE_0              (0x00U) /* timer output value is "0" */
#define _01_TAU_CH0_OUTPUT_VALUE_1              (0x01U) /* timer output value is "1" */
/* Timer output of channel 1 (TOm1) */
#define _00_TAU_CH1_OUTPUT_VALUE_0              (0x00U) /* timer output value is "0" */
#define _02_TAU_CH1_OUTPUT_VALUE_1              (0x02U) /* timer output value is "1" */
/* Timer output of channel 2 (TOm2) */
#define _00_TAU_CH2_OUTPUT_VALUE_0              (0x00U) /* timer output value is "0" */
#define _04_TAU_CH2_OUTPUT_VALUE_1              (0x04U) /* timer output value is "1" */
/* Timer output of channel 3 (TOm3) */
#define _00_TAU_CH3_OUTPUT_VALUE_0              (0x00U) /* timer output value is "0" */
#define _08_TAU_CH3_OUTPUT_VALUE_1              (0x08U) /* timer output value is "1" */

/*
    Timer Output Level Register 0 (TOLm)
*/
/* Control of timer output level of channel 1 (TOLm1) */
#define _00_TAU_CH1_OUTPUT_LEVEL_H              (0x00U) /* positive logic output (active-high) */
#define _02_TAU_CH1_OUTPUT_LEVEL_L              (0x02U) /* inverted output (active-low) */
/* Control of timer output level of channel 2 (TOLm2) */
#define _00_TAU_CH2_OUTPUT_LEVEL_H              (0x00U) /* positive logic output (active-high) */
#define _04_TAU_CH2_OUTPUT_LEVEL_L              (0x04U) /* inverted output (active-low) */
/* Control of timer output level of channel 3 (TOLm3) */
#define _00_TAU_CH3_OUTPUT_LEVEL_H              (0x00U) /* positive logic output (active-high) */
#define _08_TAU_CH3_OUTPUT_LEVEL_L              (0x08U) /* inverted output (active-low) */

/*
    Timer Output Mode Register m (TOMm)
*/
/* Control of timer output mode of channel 1 (TOMm1) */
#define _00_TAU_CH1_OUTPUT_TOGGLE               (0x00U) /* toggle operation mode */
#define _02_TAU_CH1_OUTPUT_COMBIN               (0x02U) /* combination operation mode */
/* Control of timer output mode of channel 2 (TOMm2) */
#define _00_TAU_CH2_OUTPUT_TOGGLE               (0x00U) /* toggle operation mode */
#define _04_TAU_CH2_OUTPUT_COMBIN               (0x04U) /* combination operation mode */
/* Control of timer output mode of channel 3 (TOMm3) */
#define _00_TAU_CH3_OUTPUT_TOGGLE               (0x00U) /* toggle operation mode */
#define _08_TAU_CH3_OUTPUT_COMBIN               (0x08U) /* combination operation mode */

/*
    Input Switch Control Register (ISC)
*/
 /* Switching channel 1 input of timer array unit (ISC1) */
#define _00_TAU_CH1_TI01_INPUT                  (0x00U) /* input signal of the TI01 pin as a timer input */
#define _02_TAU_CH1_RXD0_INPUT                  (0x02U) /* input signal of RxD0 pin is used as timer input */

/*
    Noise Filter Enable Register 1 (NFEN1)
*/
/* Enable/disable using noise filter of TI03 pin input signal (TNFEN03) */
#define _00_TAU_CH3_NOISE_OFF                   (0x00U) /* noise filter OFF */
#define _08_TAU_CH3_NOISE_ON                    (0x08U) /* noise filter ON */
/* Enable/disable using noise filter of TI02 pin input signal (TNFEN02) */
#define _00_TAU_CH2_NOISE_OFF                   (0x00U) /* noise filter OFF */
#define _04_TAU_CH2_NOISE_ON                    (0x04U) /* noise filter ON */
/* Enable/disable using noise filter of TI01 pin input signal (TNFEN01) */
#define _00_TAU_CH1_NOISE_OFF                   (0x00U) /* noise filter OFF */
#define _02_TAU_CH1_NOISE_ON                    (0x02U) /* noise filter ON */
/* Enable/disable using noise filter of TI00 pin input signal (TNFEN00) */
#define _00_TAU_CH0_NOISE_OFF                   (0x00U) /* noise filter OFF */
#define _01_TAU_CH0_NOISE_ON                    (0x01U) /* noise filter ON */

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* 16-bit timer data register 00 (TDR00H) */
#define _00_TAU_TDR00H_VALUE                   (0x00U)
/* 16-bit timer data register 00 (TDR00L) */
#define _00_TAU_TDR00L_VALUE                   (0x00U)
/* 16-bit timer data register 01 (TDR01H) */
#define _94_TAU_TDR01H_VALUE                   (0x94U)
/* 16-bit timer data register 01 (TDR01L) */
#define _70_TAU_TDR01L_VALUE                   (0x70U)
/* Clock divisor for TAU0 channel 0 */
#define TAU0_CHANNEL0_DIVISOR                   (1U)      /* fCLK */
/* Clock divisor for TAU0 channel 1 */
#define TAU0_CHANNEL1_DIVISOR                   (1U)      /* fCLK */
/* 16-bit timer data register 02 (TDR02) */
#define _3F_TAU_TDR02L_VALUE                    (0x3FU)
#define _9C_TAU_TDR02H_VALUE                    (0x9CU)
/* Clock divisor for TAU0 channel 2 */
#define TAU0_CHANNEL2_DIVISOR                   (1U)      /* fCLK */
/* 16-bit timer data register 03 (TDR03) */
#define _1F_TAU_TDR03L_VALUE                    (0x1FU)
#define _4E_TAU_TDR03H_VALUE                    (0x4EU)
/* Clock divisor for TAU0 channel 3 */
#define TAU0_CHANNEL3_DIVISOR                   (1U)      /* fCLK */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_TAU0_Create(void);
void R_TAU0_Channel0_Start(void);
void R_TAU0_Channel0_Stop(void);
void R_TAU0_Channel0_Set_SoftwareTriggerOn(void);
void R_TAU0_Channel2_Start(void);
void R_TAU0_Channel2_Stop(void);
void R_TAU0_Channel3_Start(void);
void R_TAU0_Channel3_Stop(void);

/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif
