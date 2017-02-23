/**
 * @file demo.c
 * @brief �f���̎���
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
	// TODO: MODE�[�q�̓ǂݎ����s���A�f����������肷��B

	demo_base_count = 0;
}

void demo_cycle_proc(void)
{
	//demo_base_count++;
	demo_base_count = 888888888;
	/*
	// ���_��
	data[0] = num_to_pattern[demo_base_count / 100 % 10];
	data[1] = num_to_pattern[demo_base_count / 10 % 10];
	data[2] = num_to_pattern[demo_base_count / 1 % 10];
	*/

	memset(data, num_to_pattern[8], 8);
	
	// ���Z���߂������̂ŁA�ȉ���1�s�ɖ�140us������
	data[0] = num_to_pattern[demo_base_count / 10000000 % 10];
	data[1] = num_to_pattern[demo_base_count / 1000000 % 10];
/*	data[2] = num_to_pattern[demo_base_count / 100000 % 10];
	data[3] = num_to_pattern[demo_base_count / 10000 % 10];
	data[4] = num_to_pattern[demo_base_count / 1000 % 10];
	data[5] = num_to_pattern[demo_base_count / 100 % 10];
	data[6] = num_to_pattern[demo_base_count / 10 % 10];
	data[7] = num_to_pattern[demo_base_count / 1 % 10];
*/
	
	light_set_data(data);
	light_set_brightness(brightness);
	
	// �{���͊O�������LATCH�M���ōX�V���邪�A
	// �f���Ȃ̂œ�������X�V����B
	light_update();
	DEBUG_PIN = 0;
}
