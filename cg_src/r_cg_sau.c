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
* File Name    : r_cg_sau.c
* Version      : Code Generator for RL78/G10 V1.04.03.03 [07 Mar 2016]
* Device(s)    : R5F10Y47
* Tool-Chain   : CCRL
* Description  : This file implements device driver for SAU module.
* Creation Date: 2017/01/22
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_sau.h"
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
volatile uint8_t * gp_csi00_rx_address;        /* csi00 receive buffer address */
volatile uint16_t  g_csi00_rx_length;          /* csi00 receive data length */
volatile uint16_t  g_csi00_rx_count;           /* csi00 receive data count */
volatile uint8_t * gp_csi00_tx_address;        /* csi00 send buffer address */
volatile uint16_t  g_csi00_send_length;        /* csi00 send data length */
volatile uint16_t  g_csi00_tx_count;           /* csi00 send data count */
volatile uint8_t * gp_csi01_rx_address;        /* csi01 receive buffer address */
volatile uint16_t  g_csi01_rx_length;          /* csi01 receive data length */
volatile uint16_t  g_csi01_rx_count;           /* csi01 receive data count */
volatile uint8_t * gp_csi01_tx_address;        /* csi01 send buffer address */
volatile uint16_t  g_csi01_send_length;        /* csi01 send data length */
volatile uint16_t  g_csi01_tx_count;           /* csi01 send data count */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_SAU0_Create
* Description  : This function initializes the SAU0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SAU0_Create(void)
{
    SAU0EN = 1U;    /* supply SAU0 clock */
    NOP();
    NOP();
    NOP();
    NOP();
    SPS0 = _00_SAU_CK00_FCLK_0 | _00_SAU_CK01_FCLK_0;
    R_CSI00_Create();
    R_CSI01_Create();
}
/***********************************************************************************************************************
* Function Name: R_CSI00_Create
* Description  : This function initializes the CSI00 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CSI00_Create(void)
{
    ST0 |= _01_SAU_CH0_STOP_TRG_ON;    /* disable CSI00 */
    CSIMK00 = 1U;    /* disable INTCSI00 interrupt */
    CSIIF00 = 0U;    /* clear INTCSI00 interrupt flag */
    /* Set INTCSI00 low priority */
    CSIPR100 = 1U;
    CSIPR000 = 1U;
    SIR00 = _02_SAU_SIRMN_PECTMN | _01_SAU_SIRMN_OVCTMN;    /* clear error flag */
    SMR00L = _20_SAU_SMRMN_INITIALVALUE | _00_SAU_MODE_CSI | _00_SAU_TRANSFER_END;
    SMR00H = _40_SAU_CLOCK_MODE_TI0N | _00_SAU_TRIGGER_SOFTWARE;
    SCR00L = _80_SAU_LSB | _07_SAU_LENGTH_8;
    SCR00H = _40_SAU_RECEPTION | _30_SAU_TIMING_4;
    SDR00H = 0U;
    SOE0 &= (uint8_t)~_01_SAU_CH0_OUTPUT_ENABLE;    /* disable CSI00 output */
    /* Set SI00 pin */
    PMC0 &= 0xFDU;
    PM0 |= 0x02U;
    /* Set SCK00 pin */
    PMC0 &= 0xFBU;
    PM0 |= 0x04U;
}
/***********************************************************************************************************************
* Function Name: R_CSI00_Start
* Description  : This function starts the CSI00 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CSI00_Start(void)
{
    SS0 |= _01_SAU_CH0_START_TRG_ON;             /* enable CSI00 */
    CSIIF00 = 0U;    /* clear INTCSI00 interrupt flag */
    CSIMK00 = 0U;    /* enable INTCSI00 */
}
/***********************************************************************************************************************
* Function Name: R_CSI00_Stop
* Description  : This function stops the CSI00 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CSI00_Stop(void)
{
    CSIMK00 = 1U;    /* disable INTCSI00 interrupt */
    ST0 |= _01_SAU_CH0_STOP_TRG_ON;        /* disable CSI00 */
    CSIIF00 = 0U;    /* clear INTCSI00 interrupt flag */
}
/***********************************************************************************************************************
* Function Name: R_CSI00_Receive
* Description  : This function receives CSI00 data.
* Arguments    : rx_buf -
*                    receive buffer pointer
*                rx_num -
*                    buffer size
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_CSI00_Receive(uint8_t * const rx_buf, uint16_t rx_num)
{
    MD_STATUS status = MD_OK;

    if (rx_num < 1U)
    {
        status = MD_ARGERROR;
    }
    else
    {
        g_csi00_rx_length = rx_num;    /* receive data length */
        g_csi00_rx_count = 0U;         /* receive data count */
        gp_csi00_rx_address = rx_buf;  /* receive buffer pointer */
    }

    return (status);
}
/***********************************************************************************************************************
* Function Name: R_CSI01_Create
* Description  : This function initializes the CSI01 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CSI01_Create(void)
{
    ST0 |= _02_SAU_CH1_STOP_TRG_ON;    /* disable CSI01 */
    CSIMK01 = 1U;    /* disable INTCSI01 interrupt */
    CSIIF01 = 0U;    /* clear INTCSI01 interrupt flag */
    /* Set INTCSI01 low priority */
    CSIPR101 = 1U;
    CSIPR001 = 1U;
    SIR01 = _04_SAU_SIRMN_FECTMN | _02_SAU_SIRMN_PECTMN | _01_SAU_SIRMN_OVCTMN;    /* clear error flag */
    SMR01L = _20_SAU_SMRMN_INITIALVALUE | _00_SAU_MODE_CSI | _00_SAU_TRANSFER_END;
    SMR01H = _00_SAU_CLOCK_SELECT_CK00 | _00_SAU_CLOCK_MODE_CKS | _00_SAU_TRIGGER_SOFTWARE;
    SCR01L = _80_SAU_LSB | _07_SAU_LENGTH_8;
    SCR01H = _80_SAU_TRANSMISSION | _30_SAU_TIMING_4;
    SDR01H = _02_CSI01_DIVISOR;
    CKO0 &= (uint8_t)~_02_SAU_CH1_CLOCK_OUTPUT_1;    /* CSI01 clock initial level */
    SO0 &= (uint8_t)~_02_SAU_CH1_DATA_OUTPUT_1;    /* CSI01 SO initial level */
    SOE0 |= _02_SAU_CH1_OUTPUT_ENABLE;    /* enable CSI01 output */
    /* Set SO01 pin */
    PMC0 &= 0xDFU;
    P0 |= 0x20U;
    PM0 &= 0xDFU;
    /* Set SCK01 pin */
    PMC0 &= 0x7FU;
    POM0 &= 0x7F;
    P0 |= 0x80U;
    PM0 &= 0x7FU;
}
/***********************************************************************************************************************
* Function Name: R_CSI01_Start
* Description  : This function starts the CSI01 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CSI01_Start(void)
{
    CKO0 &= (uint8_t)~_02_SAU_CH1_CLOCK_OUTPUT_1;   /* CSI01 clock initial level */
    SO0 &= (uint8_t)~_02_SAU_CH1_DATA_OUTPUT_1;           /* CSI01 SO initial level */
    SOE0 |= _02_SAU_CH1_OUTPUT_ENABLE;           /* enable CSI01 output */
    SS0 |= _02_SAU_CH1_START_TRG_ON;             /* enable CSI01 */
    CSIIF01 = 0U;    /* clear INTCSI01 interrupt flag */
    CSIMK01 = 0U;    /* enable INTCSI01 */
}
/***********************************************************************************************************************
* Function Name: R_CSI01_Stop
* Description  : This function stops the CSI01 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CSI01_Stop(void)
{
    CSIMK01 = 1U;    /* disable INTCSI01 interrupt */
    ST0 |= _02_SAU_CH1_STOP_TRG_ON;        /* disable CSI01 */
    SOE0 &= (uint8_t)~_02_SAU_CH1_OUTPUT_ENABLE;    /* disable CSI01 output */
    CSIIF01 = 0U;    /* clear INTCSI01 interrupt flag */
}
/***********************************************************************************************************************
* Function Name: R_CSI01_Send
* Description  : This function sends CSI01 data.
* Arguments    : tx_buf -
*                    transfer buffer pointer
*                tx_num -
*                    buffer size
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_CSI01_Send(uint8_t * const tx_buf, uint16_t tx_num)
{
    MD_STATUS status = MD_OK;

    if (tx_num < 1U)
    {
        status = MD_ARGERROR;
    }
    else
    {
        g_csi01_tx_count = tx_num;        /* send data count */
        gp_csi01_tx_address = tx_buf;     /* send buffer pointer */
        CSIMK01 = 1U;                     /* disable INTCSI01 interrupt */
        SIO01 = *gp_csi01_tx_address;    /* started by writing data to SDR[7:0] */
        gp_csi01_tx_address++;
        g_csi01_tx_count--;
        CSIMK01 = 0U;                     /* enable INTCSI01 interrupt */
    }

    return (status);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
