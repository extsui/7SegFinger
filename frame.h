/**
 * @file frame.h
 * @brief �t���[����
 *
 * Copyright (C) 2017 extsui, All rights reserved.
 */

#ifndef FRAME_H
#define FRAME_H

#include "r_cg_userdefine.h"

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

void frame_init(void);
uint8_t frame_analyze_proc(const uint8_t data[]);

#endif /* FRAME_H */
