/**
 * @file frame.c
 * @brief �t���[�����̎���
 *
 * Copyright (C) 2017 extsui, All rights reserved.
 */

#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "frame.h"

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
