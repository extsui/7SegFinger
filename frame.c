/**
 * @file frame.c
 * @brief フレーム部の実装
 *
 * Copyright (C) 2017 extsui, All rights reserved.
 */

#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "frame.h"
#include "light.h"

/************************************************************
 * プロトタイプ宣言
 ************************************************************/
/**
 * フレーム部の初期化
 * @param なし
 * @return なし
 */
void frame_init(void)
{
	light_init();
}
 
static const uint8_t ascii_to_seg_table[] = {
    0xFC,	// 0
    0x60,	// 1
    0xDA,	// 2
    0xF2,	// 3
    0x66,	// 4
    0xB6,	// 5
    0xBE,	// 6
    0xE4,	// 7
	0xFE,	// 8
	0xF6,	// 9
	/* TODO: 以降、'a'～'z'、最終的にはアスキーコード表と合わせる? */
};

static void frame_set_char(const uint8_t *data)
{
	uint8_t i = 0;
	uint8_t seg[NUM_OF_7SEG];
	
	for (i = 0; i < NUM_OF_7SEG; i++) {
		if (('0' <= data[i]) && (data[i] <= '9')) {
			seg[i] = ascii_to_seg_table[data[i] - '0'];
		} else {
			seg[i] = 0x00;
		}
	}
	
	light_set_data(seg);
}

/**
 * フレーム解析処理
 * @param [in] data 受信データ
 * @return 解析結果
 * @retval RET_OK: 成功
 * @retval RET_NG: 失敗
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
	case FRAME_CHAR:
		frame_set_char(frame->data);
		break;
	default:
		return RET_ERR;
	}
	
	return RET_OK;
}
