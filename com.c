/**
 * @file com.c
 * @brief 通信部の実装
 *
 * Copyright (C) 2017 extsui, All rights reserved.
 */

#include "r_cg_macrodriver.h"
#include "r_cg_sau.h"
#include "r_cg_intp.h"
#include "r_cg_userdefine.h"

#include "com.h"
#include "frame.h"
#include "light.h"

#pragma interrupt r_csi00_interrupt(vect=INTCSI00)
#pragma interrupt r_intc0_interrupt(vect=INTP0)
#pragma interrupt r_intc1_interrupt(vect=INTP1)

#define PIN_nCS		(P13_bit.no7)

static uint8_t frame_buf[sizeof(frame_t)];

static void enable_spi(void);
static void enable_latch(void);
static void com_receive_trigger_callback(void);

// DEBUG:
// フレーム開始トリガ(CS↓)受信回数
uint32_t debug_recv_frame_trigger_count = 0;

// DEBUG:
// フレーム受信完了回数
uint32_t debug_recv_frame_complete_count = 0;

// DEBUG:
// 受信エラー発生回数(SPI受信オーバーラン)
// デバッガ可視にするためグローバル化する。
uint32_t debug_recv_error_count = 0;

/**
 * 通信部の初期化
 */
void com_init(void)
{
	frame_init();
	enable_spi();
	enable_latch();
}

/**
 * SPI受信完了コールバック
 */
void com_received_callback(void)
{
	frame_analyze_proc(frame_buf);
}

/**
 * SPI受信トリガーコールバック
 */
void com_receive_trigger_callback(void)
{
	if (PIN_nCS == 0) {
		debug_recv_frame_trigger_count++;
		R_CSI00_Start();
		R_CSI00_Receive(frame_buf, sizeof(frame_buf));
	} else {
		R_CSI00_Stop();
	}
}

/**
 * 表示更新コールバック
 *
 * LATCHピンがアサートされた際に呼び出される。
 */
void com_update_callback(void)
{
	light_update();
}

/**
 * SPIの有効化
 */
static void enable_spi(void)
{
	R_INTC0_Start();
}

/**
 * LATCHの有効化
 */
static void enable_latch(void)
{
	R_INTC1_Start();
}

extern volatile uint8_t * gp_csi00_rx_address;         /* csi00 receive buffer address */
extern volatile uint16_t  g_csi00_rx_length;           /* csi00 receive data length */
extern volatile uint16_t  g_csi00_rx_count;            /* csi00 receive data count */
extern volatile uint8_t * gp_csi00_tx_address;         /* csi00 send buffer address */
extern volatile uint16_t  g_csi00_send_length;         /* csi00 send data length */
extern volatile uint16_t  g_csi00_tx_count;            /* csi00 send data count */

/***********************************************************************************************************************
* Function Name: r_csi00_callback_receiveend
* Description  : This function is a callback function when CSI00 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_csi00_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
	// 受信に成功しているかを確認するためのデバッグ信号。
	// コマンドによって異なるが、約30us～80us当たりの値となる。
	DEBUG_PIN = 1;
	debug_recv_frame_complete_count++;
	com_received_callback();
	DEBUG_PIN = 0;
	/* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_csi00_callback_error
* Description  : This function is a callback function when CSI00 reception error occurs.
* Arguments    : err_type -
*                    error type value
* Return Value : None
***********************************************************************************************************************/
static void r_csi00_callback_error(uint8_t err_type)
{
    /* Start user code. Do not edit comment generated here */
	// デバッグ用
	// この割り込みが入る場合、パケットロスが発生している。
	debug_recv_error_count++;
    /* End user code. Do not edit comment generated here */
}

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

    if (1U == err_type)
    {
        r_csi00_callback_error(err_type);    /* overrun error occurs */
    }
    else
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
* Function Name: r_intc0_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_intc0_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
	com_receive_trigger_callback();
	/* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_intc1_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_intc1_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
	com_update_callback();
    /* End user code. Do not edit comment generated here */
}
