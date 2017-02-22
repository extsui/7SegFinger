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

#define MODE0	(P12_bit.no1)
#define MODE1	(P12_bit.no2)

static uint32_t demo_base_count;

static uint8_t data[NUM_OF_7SEG] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };
static uint8_t brightness[NUM_OF_7SEG] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, };
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

void demo_cycle_proc(void)
{
	demo_base_count++;
		
	// 負論理
	data[0] = num_to_pattern[demo_base_count / 100 % 10];
	data[1] = num_to_pattern[demo_base_count / 10 % 10];
	data[2] = num_to_pattern[demo_base_count / 1 % 10];
	
	light_set_data(data);
	light_set_brightness(brightness);
}
