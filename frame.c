/**
 * @file frame.c
 * @brief フレーム部の実装
 *
 * Copyright (C) 2017 extsui, All rights reserved.
 */

#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "frame.h"

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
