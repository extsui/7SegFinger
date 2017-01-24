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
* File Name    : r_cg_tau.c
* Version      : Code Generator for RL78/G10 V1.04.03.03 [07 Mar 2016]
* Device(s)    : R5F10Y47
* Tool-Chain   : CCRL
* Description  : This file implements device driver for TAU module.
* Creation Date: 2017/01/24
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_tau.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_TAU0_Create
* Description  : This function initializes the TAU0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Create(void)
{
    TAU0EN = 1U;    /* supplies input clock */
    TPS0 = _00_TAU_CKM0_FCLK_0 | _00_TAU_CKM1_FCLK_0;
    /* Stop all channels */
    TTH0 =_02_TAU_CH1_H8_STOP_TRG_ON | _08_TAU_CH3_H8_STOP_TRG_ON;
    TT0 = _01_TAU_CH0_STOP_TRG_ON | _02_TAU_CH1_STOP_TRG_ON | _04_TAU_CH2_STOP_TRG_ON | _08_TAU_CH3_STOP_TRG_ON;
    /* Mask channel 0 interrupt */
    TMMK00 = 1U;    /* disable INTTM00 interrupt */
    TMIF00 = 0U;    /* clear INTTM00 interrupt flag */
    /* Mask channel 1 interrupt */
    TMMK01 = 1U;    /* disable INTTM01 interrupt */
    TMIF01 = 0U;    /* clear INTTM01 interrupt flag */
    /* Mask channel 1 higher 8 bits interrupt */
    TMMK01H = 1U;    /* disable INTTM01H interrupt */
    TMIF01H = 0U;    /* clear INTTM01H interrupt flag */
    /* Mask channel 2 interrupt */
    TMMK02 = 1U;    /* disable INTTM02 interrupt */
    TMIF02 = 0U;    /* clear INTTM02 interrupt flag */
    /* Mask channel 3 interrupt */
    TMMK03 = 1U;    /* disable INTTM03 interrupt */
    TMIF03 = 0U;    /* clear INTTM03 interrupt flag */
    /* Mask channel 3 higher 8 bits interrupt */
    TMMK03H = 1U;    /* disable INTTM03H interrupt */
    TMIF03H = 0U;    /* clear INTTM03H interrupt flag */
    /* Set INTTM02 low priority */
    TMPR102 = 1U;
    TMPR002 = 1U;
    /* Channel 0 is used as master channel for oneshot output function */
    TMR00H = _00_TAU_CLOCK_SELECT_CKM0 | _00_TAU_CLOCK_MODE_CKS | _00_TAU_TRIGGER_SOFTWARE;
    TMR00L = _08_TAU_MODE_ONESHOT;
    /* Consecutive reading from the TDR0nH and TDR0nL registers and consecutive writing to the TDR0nH and TDR0nL
    registers must be performed in the state where an interrupt is disabled by the DI instruction. */
    TDR00H = _00_TAU_TDR00H_VALUE;
    TDR00L = _00_TAU_TDR00L_VALUE;
    TO0 &= (uint8_t)~_01_TAU_CH0_OUTPUT_VALUE_1;
    TOE0 &= (uint8_t)~_01_TAU_CH0_OUTPUT_ENABLE;
    /* Channel 1 is used as slave channel for oneshot output function */
    TMR01H = _00_TAU_CLOCK_SELECT_CKM0 | _00_TAU_CLOCK_MODE_CKS | _00_TAU_COMBINATION_SLAVE | 
             _04_TAU_TRIGGER_MASTER_INT;
    TMR01L = _08_TAU_MODE_ONESHOT;
    /* Consecutive reading from the TDR0nH and TDR0nL registers and consecutive writing to the TDR0nH and TDR0nL
    registers must be performed in the state where an interrupt is disabled by the DI instruction. */
    TDR01H = _9B_TAU_TDR01H_VALUE;
    TDR01L = _78_TAU_TDR01L_VALUE;
    TOM0 |= _02_TAU_CH1_OUTPUT_COMBIN;
    TOL0 |= _02_TAU_CH1_OUTPUT_LEVEL_L;
    TO0 |= _02_TAU_CH1_OUTPUT_VALUE_1;
    TOE0 |= _02_TAU_CH1_OUTPUT_ENABLE;
    /* Channel 2 used as interval timer */
    TMR02H = _00_TAU_CLOCK_SELECT_CKM0 | _00_TAU_CLOCK_MODE_CKS | _00_TAU_COMBINATION_SLAVE | _00_TAU_TRIGGER_SOFTWARE;
    TMR02L = _00_TAU_MODE_INTERVAL_TIMER | _00_TAU_START_INT_UNUSED;
    /* Consecutive reading from the TDR0nH and TDR0nL registers and consecutive writing to the TDR0nH and TDR0nL
    registers must be performed in the state where an interrupt is disabled by the DI instruction. */
    TDR02H = _9C_TAU_TDR02H_VALUE;
    TDR02L = _3F_TAU_TDR02L_VALUE;
    TOM0 &= (uint8_t)~_04_TAU_CH2_OUTPUT_COMBIN;
    TOL0 &= (uint8_t)~_04_TAU_CH2_OUTPUT_LEVEL_L;
    TO0 &= (uint8_t)~_04_TAU_CH2_OUTPUT_VALUE_1;
    TOE0 &= (uint8_t)~_04_TAU_CH2_OUTPUT_ENABLE;
    /* Set TO01 pin */
    PMC0 &= 0xEFU;
    P0 &= 0xEFU;
    PM0 &= 0xEFU;
}
/***********************************************************************************************************************
* Function Name: R_TAU0_Channel0_Start
* Description  : This function starts TAU0 channel 0 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Channel0_Start(void)
{
    TOE0 |= _02_TAU_CH1_OUTPUT_ENABLE;
    TS0 |= _01_TAU_CH0_START_TRG_ON | _02_TAU_CH1_START_TRG_ON;
}
/***********************************************************************************************************************
* Function Name: R_TAU0_Channel0_Stop
* Description  : This function stops TAU0 channel 0 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Channel0_Stop(void)
{
    TT0 |= _01_TAU_CH0_STOP_TRG_ON | _02_TAU_CH1_STOP_TRG_ON;
    TOE0 &= (uint8_t)~_02_TAU_CH1_OUTPUT_ENABLE;
}
/***********************************************************************************************************************
* Function Name: R_TAU0_Channel0_Set_SoftwareTriggerOn
* Description  : This function generates software trigger for One-shot output function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Channel0_Set_SoftwareTriggerOn(void)
{
    TS0 |= _01_TAU_CH0_START_TRG_ON;
}
/***********************************************************************************************************************
* Function Name: R_TAU0_Channel2_Start
* Description  : This function starts TAU0 channel 2 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Channel2_Start(void)
{
    TMIF02 = 0U;    /* clear INTTM02 interrupt flag */
    TMMK02 = 0U;    /* enable INTTM02 interrupt */
    TS0 |= _04_TAU_CH2_START_TRG_ON;
}
/***********************************************************************************************************************
* Function Name: R_TAU0_Channel2_Stop
* Description  : This function stops TAU0 channel 2 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Channel2_Stop(void)
{
    TT0 |= _04_TAU_CH2_STOP_TRG_ON;
    /* Mask channel 2 interrupt */
    TMMK02 = 1U;    /* disable INTTM02 interrupt */
    TMIF02 = 0U;    /* clear INTTM02 interrupt flag */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
