/**
 * @file com.c
 * @brief �ʐM���̎���
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
// �t���[���J�n�g���K(CS��)��M��
uint32_t debug_recv_frame_trigger_count = 0;

// DEBUG:
// �t���[����M������
uint32_t debug_recv_frame_complete_count = 0;

// DEBUG:
// ��M�G���[������(SPI��M�I�[�o�[����)
// �f�o�b�K���ɂ��邽�߃O���[�o��������B
uint32_t debug_recv_error_count = 0;

/**
 * �ʐM���̏�����
 */
void com_init(void)
{
	frame_init();
	enable_spi();
	enable_latch();
}

/**
 * SPI��M�����R�[���o�b�N
 */
void com_received_callback(void)
{
	frame_analyze_proc(frame_buf);
}

/**
 * SPI��M�g���K�[�R�[���o�b�N
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
 * �\���X�V�R�[���o�b�N
 *
 * LATCH�s�����A�T�[�g���ꂽ�ۂɌĂяo�����B
 */
void com_update_callback(void)
{
	light_update();
}

/**
 * SPI�̗L����
 */
static void enable_spi(void)
{
	R_INTC0_Start();
}

/**
 * LATCH�̗L����
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
	// ��M�ɐ������Ă��邩���m�F���邽�߂̃f�o�b�O�M���B
	// �R�}���h�ɂ���ĈقȂ邪�A��30us�`80us������̒l�ƂȂ�B
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
	// �f�o�b�O�p
	// ���̊��荞�݂�����ꍇ�A�p�P�b�g���X���������Ă���B
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
