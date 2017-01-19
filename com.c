/**
 * @file com.c
 * @brief í êMïîÇÃé¿ëï
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

static void do_ack_response(void);
static void assert_cs(void);
static void negate_cs(void);

void com_init(void)
{
	PIN_ACK = 0;
	assert_cs();
}

void com_received_callback(void)
{
	frame_analyze_proc(frame_buf);
}

void com_receive_trigger_callback(void)
{
	if (PIN_nCS == 0) {
		R_CSI00_Start();
		R_CSI00_Receive(frame_buf, sizeof(frame_buf));
	} else {
		R_CSI00_Stop();
	}
}

static void assert_cs(void)
{
	R_INTC0_Start();
}

static void negate_cs(void)
{
	R_INTC0_Stop();	
}
