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

#define PIN_nCS		(P13_bit.no7)

static uint8_t frame_buf[sizeof(frame_t)];

static void enable_spi(void);
static void enable_latch(void);

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
