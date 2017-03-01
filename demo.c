/**
 * @file demo.c
 * @brief デモの実装
 *
 * Copyright (C) 2017 extsui, All rights reserved.
 */

#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "light.h"
#include "demo.h"

#include <string.h>

#define MODE0	(P12_bit.no1)
#define MODE1	(P12_bit.no2)

#define SCROLL_DELAY_MS	(200)	// スクロール遅延ミリ秒

static uint32_t demo_base_count;

static uint8_t data[NUM_OF_7SEG] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };
static uint8_t brightness[NUM_OF_7SEG] = { 100, 100, 100, 100, 100, 100, 100, 100, };
static const uint8_t num_to_pattern[] = {
	0xfc, /* 0 */
	0x60, /* 1 */
	0xda, /* 2 */
	0xf2, /* 3 */
	0x66, /* 4 */
	0xb6, /* 5 */
	0xbe, /* 6 */
	0xe4, /* 7 */
	0xfe, /* 8 */
	0xf6, /* 9 */
};

void demo_init(void)
{
	// TODO: MODE端子の読み取りを行い、デモ動作を決定する。

	demo_base_count = 0;
}

static uint8_t banner[] = {
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	
	0xe4, /* 7 */
	0xb6, /* S */
	0x9e, /* E */
	0xbc, /* G */
	0x10, /* _ */
	0x8e, /* F */
	0x60, /* I */
	0xec, /* N */
	0xbc, /* G */
	0x9e, /* E */
	0xef, /* R */
	
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
};

void demo_cycle_proc(void)
{
	static int pos = 0;
	
	demo_base_count++;

	if (demo_base_count % SCROLL_DELAY_MS == 0) {
		pos++;
		if (pos == ((sizeof(banner) / sizeof(banner[0])) - 8 + 1)) {
			pos = 0;
		}
	}
	
	memcpy(data, &banner[pos], sizeof(data));
	
	light_set_data(data);
	light_set_brightness(brightness);
	
	// 本来は外部からのLATCH信号で更新するが、
	// デモなので内部から更新する。
	light_update();
}
