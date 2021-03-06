/**
 * @file frame.h
 * @brief フレーム部
 *
 * Copyright (C) 2017 extsui, All rights reserved.
 */

#ifndef FRAME_H
#define FRAME_H

#include "r_cg_userdefine.h"

/** フレーム種別 */
typedef enum {
	FRAME_DATA = 1,			///< 7セグ表示データ指定
	FRAME_BRIGHTNESS = 2,	///< 7セグ輝度データ指定
	FRAME_CHAR = 3,			///< 7セグ文字データ指定
	FRMAE_SETTING = 4,		///< 7セグ設定データ指定
} frame_type_t;

/** フレーム構造体 */
typedef struct {
	frame_type_t type;			///< 種別
	uint8_t data[NUM_OF_7SEG];	///< 各7セグ用データ
} frame_t;

void frame_init(void);
uint8_t frame_analyze_proc(const uint8_t data[]);

#endif /* FRAME_H */
