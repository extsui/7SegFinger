/**
 * @file light.c
 * @brief �_�����̎���
 *
 * Copyright (C) 2017 extsui, All rights reserved.
 */

#include "r_cg_macrodriver.h"
#include "r_cg_tau.h"
#include "r_cg_sau.h"
#include "r_cg_userdefine.h"
#include "iodefine.h"

#include "light.h"
#include <string.h>

#define PIN_nSCLR	(P0_bit.no3)
#define PIN_RCK		(P0_bit.no6)

/** �_���p�f�[�^�\���� */
typedef struct {
	uint8_t data;		///< �\���f�[�^
	uint8_t brightness;	///< �P�x(0-100)
} light_t;

/** �_�����f�[�^��� */
static light_t light[NUM_OF_7SEG] = {
	{ 0x60, 100, },
	{ 0xDA, 6, }, // TODO: �Œ�P�x�͕ۏ؂������B�܂�A�P�x=1�ł����Â��\���͂��Ă��������B
	{ 0xF2, 0, }, //		TDR�X�V�O�Ƀ^�C�}1�~�߂āATDR�X�V��Ƀ^�C�}�X�^�[�g�Ŏ����\�H
	{ 0xFE, 0, },
	{ 0xFE, 0, },
	{ 0xFE, 0, },
	{ 0xFE, 0, },
	{ 0xFE, 0, },
};

/** �X�V�p�f�[�^��� */
static light_t latch[NUM_OF_7SEG];

/** ���݂̓_���ӏ� */
static int light_cur_pos;

/**
 * �_�����̏�����
 */
void light_init(void)
{
	PIN_nSCLR = 0;
	PIN_nSCLR = 1;
	PIN_RCK = 0;
	
	light_cur_pos = 0;
	
	R_CSI01_Start();
	R_TAU0_Channel0_Start();
}

/**
 * �\���f�[�^�̐ݒ�
 */
void light_set_data(const uint8_t data[])
{
	int i;
	for (i = 0; i < NUM_OF_7SEG; i++) {
		latch[i].data = data[i];
	}
}

/**
 * �P�x�̐ݒ�
 */
void light_set_brightness(const uint8_t brightness[])
{
	int i;
	for (i = 0; i < NUM_OF_7SEG; i++) {
		latch[i].brightness = brightness[i];
	}
}

/**
 * �\�����f�[�^��ݒ肵���f�[�^�ɍX�V����
 */
void light_update(void)
{
	memcpy(light, latch, sizeof(light));
}

/**
 * �\���؂�ւ��R�[���o�b�N
 *
 * ���ݓ_�����Ă���7�Z�O��؂�ւ���
 */
void light_move_to_next_pos_callback(void)
{
	static uint8_t shift_data[sizeof(light_t)];
	static const uint8_t bit_table[] = {
		0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01,
	};

	shift_data[0] = bit_table[light_cur_pos];
	shift_data[1] = light[light_cur_pos].data;

	R_CSI01_Send(shift_data, sizeof(shift_data));

	// TODO: WORKAROUND:
	// �Ȃ��light_update_callback()�̍Ō�Ɏ����Ă�������P�x��1����������邩�킩���B
	// �Ԃ����Ⴏ�R�[���o�b�N�̌Ăяo���������킩���ĂȂ��B
	light_cur_pos = (light_cur_pos + 1) % NUM_OF_7SEG;
}

/**
 * �V�t�g���W�X�^�̕\���X�V�R�[���o�b�N
 *
 * 7�Z�O�̕\���f�[�^�𔽉f����
 */
void light_update_shift_register_callback(void)
{
	uint16_t off_timing;
	uint8_t new_tdr01h, new_tdr01l;
	
	// TDR0nH��TDR0nL�̏��ԂɘA���ŏ������ޕK�v������
	uint32_t temp;

	DI();
	
	temp = (uint32_t)0x9C40 * light[light_cur_pos].brightness;
	off_timing = (uint16_t)(temp / 100);
	new_tdr01h = (uint8_t)((off_timing & 0xFF00) >> 8);
	new_tdr01l = (uint8_t)(off_timing & 0x00FF);
	TDR01H = new_tdr01h;
	TDR01L = new_tdr01l;
	
	EI();
	
	PIN_RCK = 1;
	PIN_RCK = 0;
}
