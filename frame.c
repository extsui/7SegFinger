/**
 * @file frame.c
 * @brief �t���[�����̎���
 *
 * Copyright (C) 2017 extsui, All rights reserved.
 */

#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "frame.h"

/** �t���[����� */
typedef enum {
	FRAME_DATA = 0,			///< 7�Z�O�\���f�[�^�w��
	FRAME_BRIGHTNESS = 1,	///< 7�Z�O�P�x�f�[�^�w��
} frame_type_t;

/** �t���[���\���� */
typedef struct {
	frame_type_t type;			///< ���
	uint8_t data[NUM_OF_7SEG];	///< �e7�Z�O�p�f�[�^
	uint8_t checksum;			///< �`�F�b�N�T��
} frame_t;

/** �t���[���T�C�Y(�O�����J�p) */
const int g_frame_size = sizeof(frame_t);

/************************************************************
 * �v���g�^�C�v�錾
 ************************************************************/
static uint8_t calc_checksum(const frame_t *frame);

/**
 * �t���[�����̏�����
 * @param �Ȃ�
 * @return �Ȃ�
 */
void frame_init(void)
{
	/* NOP */
}

/**
 * �t���[������
 * @param [in] data ��M�f�[�^
 * @return ��͌���
 * @retval RET_OK: ����
 * @retval RET_NG: ���s
 */
uint8_t frame_analyze_proc(const uint8_t data[])
{
	return RET_OK;
}

static uint8_t calc_checksum(const frame_t *frame)
{
	return 0;
}
