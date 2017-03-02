/**
 * @file mode.c
 * @brief ���[�h�̎���
 *
 * Copyright (C) 2017 extsui, All rights reserved.
 */

#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "light.h"
#include "mode.h"

#include <string.h>

/**
 * ���[�h�[�q
 *
 * DIP�X�C�b�`�ɐڑ�����Ă���B
 * ���I�Ƀ��[�h�̐؂�ւ��ɑΉ�����B
 */
#define MODE0	(P12_bit.no1)
#define MODE1	(P12_bit.no2)

#define SCROLL_DELAY_MS	(200)	// �X�N���[���x���~���b

static uint8_t get_mode(void);
static void brightness_max_and_all_light_demo(void);
static void controll_brightness_demo(void);
static void presentation_demo(void);
static void nop(void);

/**
 * ����e�[�u��
 *
 * -----+----------------------
 * MODE |      ���샂�[�h
 * -----+----------------------
 *  00  | �f��1(�P�x�ő�S�_��)
 *  01  | �f��2(�P�x�����f��)
 *  10  | �f��3(�W���p�f��)
 *  11  | �O�����䃂�[�h
 * -----+----------------------
 */
typedef void (*action_t)(void);
static const action_t action_table[] = {
	brightness_max_and_all_light_demo,
	controll_brightness_demo,
	presentation_demo,
	nop,
};

static uint32_t demo_base_count;

static uint8_t cur_mode = 0;

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

/************************************************************
 * ���J�֐�
 ************************************************************/

 /**
  * @par �������e
  * - ���[�h��ݒ肷��B
  */
void mode_init(void)
{
	mode_update();
}

/**
 * @par �������e
 * - ���݂̃��[�h�p�̃A�N�V�����֐����Ăяo���B
 */
void mode_proc(void)
{
	demo_base_count++;
	action_table[cur_mode]();
}

/**
 * @par �������e
 * - ���[�h�[�q��ǂݎ��B
 * - ���݂̃��[�h�ƈقȂ��Ă����ꍇ�A���[�h���X�V����B
 * - �X�V����ꍇ�A�f���p�J�E���^�͏���������B
 */
void mode_update(void)
{
	uint8_t new_mode = get_mode();
	if (cur_mode != new_mode) {
		cur_mode = new_mode;
		demo_base_count = 0;
	}
}

/************************************************************
 * �����֐�
 ************************************************************/

static uint8_t get_mode(void)
{
	return ((MODE1<<1) | MODE0);
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

/**
 * [8][8][8][8][8][8][8][8]
 */
static void brightness_max_and_all_light_demo(void)
{	
	memset(data, 0xff, sizeof(data));
	memset(brightness, 100, sizeof(brightness));
	
	light_set_data(data);
	light_set_brightness(brightness);
	light_update();
}

/**
 * [%][3][d][ ][8][8][8][8]
 *
 * �O��3���ŋP�x(0-100)��\���A�㔼4���ŋP�x�ݒ�B
 * �u0��100(�J�E���g�A�b�v)�v���u100��0(�J�E���g�_�E��)�v���J��Ԃ��B
 * 10�~���b���ɁA1���J�E���g�A�b�v/�J�E���g�_�E������B
 */
static void controll_brightness_demo(void)
{
	BOOL count_update_timing_flag;
	BOOL count_up_phase_flag;
	uint32_t brightness_value;
	
	// �P�x�v�Z
	// ��1: 1234[ms] --> 23
	// ��2: 7890[ms] --> 89
	brightness_value = demo_base_count / 10 % 100;
	
	// �J�E���g�X�V�^�C�~���O��
	// ��1: 1234[ms] --> 4 --> FALSE
	// ��2: 7890[ms] --> 0 --> TRUE
	count_update_timing_flag = ((demo_base_count % 10) == 0) ? TRUE : FALSE;
	if (!count_update_timing_flag) {
		return;
	}
	
	// �J�E���g�A�b�v�t�F�[�Y��
	// �N����(0�`1000[ms])�̓J�E���g�A�b�v����n�܂��Ăق����̂ŁA
	// �����b����b���J�E���g�A�b�v�Ƃ���B
	// ��1: 1234[ms] --> 1 --> 1 --> FALSE
	// ��2: 7890[ms] --> 7 --> 1 --> FALSE
	count_up_phase_flag = (((demo_base_count / 1000) % 2) == 0) ? TRUE : FALSE;
	if (!count_up_phase_flag) {
		brightness_value = 100 - brightness_value;
	}

	// "%3d"�B�擪��0�̏ꍇ�͕\�����Ȃ��B
	data[0] = num_to_pattern[brightness_value / 100 % 10];
	if (data[0] == num_to_pattern[0]) {
		data[0] = 0x00;
	}
	data[1] = num_to_pattern[brightness_value / 10 % 10];
	if (data[1] == num_to_pattern[0]) {
		if (data[0] == 0x00) {
			data[1] = 0x00;
		}
	}
	data[2] = num_to_pattern[brightness_value / 1 % 10];
	data[3] = 0;
	data[4] = 0xff;
	data[5] = 0xff;
	data[6] = 0xff;
	data[7] = 0xff;
	
	brightness[0] = 100;
	brightness[1] = 100;
	brightness[2] = 100;
	brightness[3] = 100;
	brightness[4] = brightness_value;
	brightness[5] = brightness_value;
	brightness[6] = brightness_value;
	brightness[7] = brightness_value;
	
	light_set_data(data);
	light_set_brightness(brightness);
	light_update();
}

/**
 * [7][S][E][G][_][F][I][N][G][E][R]���X�N���[���\��
 */
static void presentation_demo(void)
{
	// TODO: �������֐����쐬���ă��[�h�ύX�̓x�ɏ����ʒu����n�߂����B
	static int pos = 0;
	
	if (demo_base_count % SCROLL_DELAY_MS == 0) {
		pos++;
		if (pos == ((sizeof(banner) / sizeof(banner[0])) - 8 + 1)) {
			pos = 0;
		}
	}
	
	memcpy(data, &banner[pos], sizeof(data));
	memset(brightness, 100, sizeof(brightness));
	
	light_set_data(data);
	light_set_brightness(brightness);
	
	// �{���͊O�������LATCH�M���ōX�V���邪�A
	// �f���Ȃ̂œ�������X�V����B
	light_update();
}

static void nop(void)
{
	/* �������Ȃ� */
}
