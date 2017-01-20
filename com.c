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

#include "frame.h"
#include "com.h"

#define PIN_ACK	(P0_bit.no0)
#define PIN_nCS	(P13_bit.no7)

static uint8_t frame_buf[sizeof(frame_t)];

static void enable_spi(void);
static void do_ack_response(void);

/**
 * �ʐM���̏�����
 */
void com_init(void)
{
	frame_init();
	
	PIN_ACK = 0;
	enable_spi();
}

/**
 * SPI��M�����R�[���o�b�N
 */
void com_received_callback(void)
{
	uint8_t result;
	result = frame_analyze_proc(frame_buf);
	if (result == RET_OK) {
		do_ack_response();
	}
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
 * SPI�̗L����
 */
static void enable_spi(void)
{
	R_INTC0_Start();
}

/**
 * ACK����
 */
static void do_ack_response(void)
{
	PIN_ACK = 1;
	NOP();
	PIN_ACK = 0;
}
