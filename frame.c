/**
 * @file frame.c
 * @brief �t���[�����̎���
 *
 * Copyright (C) 2017 extsui, All rights reserved.
 */

#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "frame.h"
#include "light.h"

/************************************************************
 * �v���g�^�C�v�錾
 ************************************************************/
/**
 * �t���[�����̏�����
 * @param �Ȃ�
 * @return �Ȃ�
 */
void frame_init(void)
{
	light_init();
}

/**
 * �t���[����͏���
 * @param [in] data ��M�f�[�^
 * @return ��͌���
 * @retval RET_OK: ����
 * @retval RET_NG: ���s
 */
uint8_t frame_analyze_proc(const uint8_t data[])
{
	const frame_t *frame = (const frame_t*)data;
	
	switch (frame->type) {
	case FRAME_DATA:
		light_set_data(frame->data);
		break;
	case FRAME_BRIGHTNESS:
		light_set_brightness(frame->data);
		break;
	default:
		return RET_ERR;
	}
	
	return RET_OK;
}
