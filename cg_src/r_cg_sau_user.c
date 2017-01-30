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
* File Name    : r_cg_sau_user.c
* Version      : Code Generator for RL78/G10 V1.04.03.03 [07 Mar 2016]
* Device(s)    : R5F10Y47
* Tool-Chain   : CCRL
* Description  : This file implements device driver for SAU module.
* Creation Date: 2017/01/30
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_sau.h"
/* Start user code for include. Do not edit comment generated here */
#include "com.h"
#include "light.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt r_csi00_interrupt(vect=INTCSI00)
#pragma interrupt r_csi01_interrupt(vect=INTCSI01)
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint8_t * gp_csi00_rx_address;         /* csi00 receive buffer address */
extern volatile uint16_t  g_csi00_rx_length;           /* csi00 receive data length */
extern volatile uint16_t  g_csi00_rx_count;            /* csi00 receive data count */
extern volatile uint8_t * gp_csi00_tx_address;         /* csi00 send buffer address */
extern volatile uint16_t  g_csi00_send_length;         /* csi00 send data length */
extern volatile uint16_t  g_csi00_tx_count;            /* csi00 send data count */
extern volatile uint8_t * gp_csi01_rx_address;         /* csi01 receive buffer address */
extern volatile uint16_t  g_csi01_rx_length;           /* csi01 receive data length */
extern volatile uint16_t  g_csi01_rx_count;            /* csi01 receive data count */
extern volatile uint8_t * gp_csi01_tx_address;         /* csi01 send buffer address */
extern volatile uint16_t  g_csi01_send_length;         /* csi01 send data length */
extern volatile uint16_t  g_csi01_tx_count;            /* csi01 send data count */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_csi00_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_csi00_interrupt(void)
{
    volatile uint8_t err_type;

    err_type = (uint8_t)(SSR00 & _01_SAU_OVERRUN_ERROR);
    SIR00 = (uint8_t)err_type;

    if (err_type != 1U)
    {
        if (g_csi00_rx_count < g_csi00_rx_length)
        {
            *gp_csi00_rx_address = SIO00;
            gp_csi00_rx_address++;
            g_csi00_rx_count++;

            if (g_csi00_rx_count == g_csi00_rx_length)
            {
                r_csi00_callback_receiveend();    /* complete receive */
            }
        }
    }
}
/***********************************************************************************************************************
* Function Name: r_csi00_callback_receiveend
* Description  : This function is a callback function when CSI00 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_csi00_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
	com_received_callback();
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_csi01_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_csi01_interrupt(void)
{
    volatile uint8_t err_type;

    err_type = (uint8_t)(SSR01 & _01_SAU_OVERRUN_ERROR);
    SIR01 = (uint8_t)err_type;

    if (err_type != 1U)
    {
        if (g_csi01_tx_count > 0U)
        {
            SIO01 = *gp_csi01_tx_address;
            gp_csi01_tx_address++;
            g_csi01_tx_count--;
        }
        else
        {
            r_csi01_callback_sendend();    /* complete send */
        }
    }
}
/***********************************************************************************************************************
* Function Name: r_csi01_callback_sendend
* Description  : This function is a callback function when CSI01 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_csi01_callback_sendend(void)
{
    /* Start user code. Do not edit comment generated here */
	light_update_shift_register_callback();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
