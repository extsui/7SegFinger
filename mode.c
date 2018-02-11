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

//#define SCROLL_DELAY_MS	(200)	// �X�N���[���x���~���b
#define SCROLL_DELAY_MS	(100)	// �X�N���[���x���~���b

static uint8_t get_mode(void);
static void nop(void);
static void update(void);

static void brightness_max_and_turn_on_all(void);
static void brightness_min_and_turn_off_all(void);
static void controll_brightness_demo(void);
static void presentation_demo_init(void);
static void presentation_demo(void);

typedef void (*action_t)(void);

/**
 * ��ԗp�̃A�N�V�����\����
 */
typedef struct {
	action_t entry;	/**< ��Ԃɓ��������̃A�N�V���� */
	action_t proc;	/**< ��Ԃ̒���1ms���ɌĂяo�����A�N�V���� */
} state_action_t;

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
static const state_action_t state_table[] = {
	{ brightness_max_and_turn_on_all,	nop,						},
	{ nop,								controll_brightness_demo,	},
	{ presentation_demo_init,			presentation_demo,			},
	{ brightness_min_and_turn_off_all,	nop,						},
};

static uint32_t demo_base_count = 0;

/**
 * ���݂̃��[�h
 * 
 * �N�����ɏ������������K�����s�����悤�ɂ��邽�߁A
 * ���[�h�[�q�ł��蓾�Ȃ��l(0-3�ȊO)��ݒ肵�Ă����B
 */
static uint8_t cur_mode = 0xFF;

static uint8_t data[NUM_OF_7SEG] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };
static uint8_t brightness[NUM_OF_7SEG] = { 255, 255, 255, 255, 255, 255, 255, 255, };
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
	state_table[cur_mode].proc();
	demo_base_count++;
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
		state_table[cur_mode].entry();
	}
}

/************************************************************
 * �����֐�
 ************************************************************/

static uint8_t get_mode(void)
{
	uint8_t mode = ((MODE1<<1) | MODE0);
	return mode;
}

static void nop(void)
{
	/* �������Ȃ� */
}

/**
 * ����̃f�[�^�ƋP�x�ŕ\�����X�V����B
 */
static void update(void)
{
	light_set_data(data);
	light_set_brightness(brightness);
	
	// �{���͊O�������LATCH�M���ōX�V���邪�A
	// �f���Ȃ̂œ�������X�V����B
	light_update();
}

/************************************************************/

/**
 * [8][8][8][8][8][8][8][8]
 */
static void brightness_max_and_turn_on_all(void)
{
	memset(data, 0xff, sizeof(data));
	memset(brightness, 255, sizeof(brightness));
	update();
}

/**
 * [ ][ ][ ][ ][ ][ ][ ][ ]
 */
static void brightness_min_and_turn_off_all(void)
{
	memset(data, 0x00, sizeof(data));
	memset(brightness, 0, sizeof(brightness));
	update();
}

/************************************************************/

/**
 * [%][3][d][ ][8][8][8][8]
 *
 * �O��3���ŋP�x(0-255)��\���A�㔼4���ŋP�x�ݒ�B
 * �u0��255(�J�E���g�A�b�v)�v���u255��0(�J�E���g�_�E��)�v���J��Ԃ��B
 * 10�~���b���ɁA1���J�E���g�A�b�v/�J�E���g�_�E������B
 */
static void controll_brightness_demo(void)
{
	BOOL count_update_timing_flag;
	BOOL count_up_phase_flag;
	uint32_t brightness_value;
	
	// �P�x�v�Z
	// �J�E���^��10ms�̌���1�̈ʂƂ��āA����8�r�b�g���P�x�Ƃ��Ďg�p����B
	brightness_value = (demo_base_count / 10) & 0xFF;
	
	// �J�E���g�X�V�^�C�~���O��z
	// ��1: 1234[ms] --> 4 --> FALSE
	// ��2: 7890[ms] --> 0 --> TRUE
	count_update_timing_flag = ((demo_base_count % 10) == 0) ? TRUE : FALSE;
	if (!count_update_timing_flag) {
		return;
	}
	
	// �J�E���g�A�b�v�t�F�[�Y��
	// �N�����̓J�E���g�A�b�v����n�܂��Ăق����̂ŁA
	// �����b����b���J�E���g�A�b�v�Ƃ���B
	count_up_phase_flag = ((((demo_base_count / 10) >> 8) & 0x1) == 0) ? TRUE : FALSE;
	if (!count_up_phase_flag) {
		brightness_value = 255 - brightness_value;
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
	
	brightness[0] = 255;
	brightness[1] = 255;
	brightness[2] = 255;
	brightness[3] = 255;
	brightness[4] = brightness_value;
	brightness[5] = brightness_value;
	brightness[6] = brightness_value;
	brightness[7] = brightness_value;
	
	update();
}

/************************************************************/

/** �W���p�f���̃o�i�[ */
//static const uint8_t banner[] = {
/*	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
*/

//	0x40,
//	0xe4, /* 7 */
//	0x4d,
//	0xb6, /* S */
//	0x69,
//	0x9e, /* E */
//	0x61,
//	0xbc, /* G */
//	0x09,
//	0x10, /* _ */
//	0x60,	
//	0x8e, /* F */
//	0x00,
//	0x60, /* I */
//	0x6c,
//	0xec, /* N */
//	0x6d,
//	0xbc, /* G */
//	0x69,
//	0x9e, /* E */
//	0x60,
//	0xef, /* R */
//	0x0c,
/*	
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
};
*/

static const uint8_t banner[] = {
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

static const uint8_t banner_virtual[] = {
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,

//	0x40,
//	0x0c,
	
	0x40, /*  +7 */
	0x4d, /* 7+S */
	0x69, /* S+E */
	0x61, /* E+G */
	0x09, /* G+_ */
	0x60, /* _+F */
	0x00, /* F+I */
	0x6c, /* I+N */
	0x6d, /* N+G */
	0x69, /* G+E */
	0x60, /* E+R */
	0x0c, /* R+  */

	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
};


/** �X�N���[���ʒu(�W���p�f���Ŏg�p) */
static int scroll_pos;

/** �X�N���[���^�C�~���O */
static uint32_t next_scroll_time;

static void presentation_demo_init(void)
{
	scroll_pos = 0;
	next_scroll_time = demo_base_count + SCROLL_DELAY_MS;
}

/**
 * [7][S][E][G][_][F][I][N][G][E][R]���X�N���[���\��
 */
static void presentation_demo(void)
{
	if (demo_base_count >= next_scroll_time) {
		static BOOL virtual_timing = FALSE;
	
		next_scroll_time += SCROLL_DELAY_MS;
		
		if (!virtual_timing) {
			memcpy(data, &banner[scroll_pos], sizeof(data));
			virtual_timing = TRUE;
		} else {
			scroll_pos++;
			memcpy(data, &banner_virtual[scroll_pos], sizeof(data));
			virtual_timing = FALSE;
		}
		
		if (scroll_pos == ((sizeof(banner) / sizeof(banner[0])) - 8 + 1)) {
			scroll_pos = 0;
		}
	}
	
	memset(brightness, 255, sizeof(brightness));
	
	update();
}
