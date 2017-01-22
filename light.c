/**
 * @file light.c
 * @brief 点灯部の実装
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

/** 点灯用データ構造体 */
typedef struct {
	uint8_t data;		///< 表示データ
	uint8_t brightness;	///< 輝度(0-100)
} light_t;

/** 点灯中データ情報 */
static light_t light[NUM_OF_7SEG] = {
	{ 0x60, 100, },
	{ 0xDA, 6, }, // TODO: 最低輝度は保証したい。つまり、輝度=1でも薄暗く表示はしておきたい。
	{ 0xF2, 0, }, //		TDR更新前にタイマ1止めて、TDR更新後にタイマスタートで実現可能？
	{ 0xFE, 0, },
	{ 0xFE, 0, },
	{ 0xFE, 0, },
	{ 0xFE, 0, },
	{ 0xFE, 0, },
};

/** 更新用データ情報 */
static light_t latch[NUM_OF_7SEG];

/** 現在の点灯箇所 */
static int light_cur_pos;

/**
 * 点灯部の初期化
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
 * 表示データの設定
 */
void light_set_data(const uint8_t data[])
{
	int i;
	for (i = 0; i < NUM_OF_7SEG; i++) {
		latch[i].data = data[i];
	}
}

/**
 * 輝度の設定
 */
void light_set_brightness(const uint8_t brightness[])
{
	int i;
	for (i = 0; i < NUM_OF_7SEG; i++) {
		latch[i].brightness = brightness[i];
	}
}

/**
 * 表示中データを設定したデータに更新する
 */
void light_update(void)
{
	memcpy(light, latch, sizeof(light));
}

/**
 * 表示切り替えコールバック
 *
 * 現在点灯している7セグを切り替える
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
	// なんでlight_update_callback()の最後に持っていったら輝度が1周期分ずれるかわからん。
	// ぶっちゃけコールバックの呼び出し順序がわかってない。
	light_cur_pos = (light_cur_pos + 1) % NUM_OF_7SEG;
}

/**
 * シフトレジスタの表示更新コールバック
 *
 * 7セグの表示データを反映する
 */
void light_update_shift_register_callback(void)
{
	uint16_t off_timing;
	uint8_t new_tdr01h, new_tdr01l;
	
	// TDR0nH→TDR0nLの順番に連続で書き込む必要がある
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
