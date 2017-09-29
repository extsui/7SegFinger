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
	default:
		return RET_ERR;
	}
	
	return RET_OK;
}
