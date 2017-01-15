/**
 * @file frame.c
 * @brief フレーム部の実装
 *
 * Copyright (C) 2017 extsui, All rights reserved.
 */

#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "frame.h"

/** フレーム種別 */
typedef enum {
	FRAME_DATA = 0,			///< 7セグ表示データ指定
	FRAME_BRIGHTNESS = 1,	///< 7セグ輝度データ指定
} frame_type_t;

/** フレーム構造体 */
typedef struct {
	frame_type_t type;			///< 種別
	uint8_t data[NUM_OF_7SEG];	///< 各7セグ用データ
	uint8_t checksum;			///< チェックサム
} frame_t;

/** フレームサイズ(外部公開用) */
const int g_frame_size = sizeof(frame_t);

/************************************************************
 * プロトタイプ宣言
 ************************************************************/
static uint8_t calc_checksum(const frame_t *frame);

/**
 * フレーム部の初期化
 * @param なし
 * @return なし
 */
void frame_init(void)
{
	/* NOP */
}

/**
 * フレーム処理
 * @param [in] data 受信データ
 * @return 解析結果
 * @retval RET_OK: 成功
 * @retval RET_NG: 失敗
 */
uint8_t frame_analyze_proc(const uint8_t data[])
{
	return RET_OK;
}

static uint8_t calc_checksum(const frame_t *frame)
{
	return 0;
}
