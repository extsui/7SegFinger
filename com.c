/**
 * @file com.c
 * @brief ’ÊM•”‚ÌÀ‘•
 *
 * Copyright (C) 2017 extsui, All rights reserved.
 */

#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"

#include "com.h"

#define PIN_ACK	(P0_bit.no0)

static void do_ack_response(void);

void com_init(void)
{
	PIN_ACK = 0;
}
