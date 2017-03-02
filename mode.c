/**
 * @file mode.c
 * @brief モードの実装
 *
 * Copyright (C) 2017 extsui, All rights reserved.
 */

#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "light.h"
#include "mode.h"

#include <string.h>

/**
 * モード端子
 *
 * DIPスイッチに接続されている。
 * 動的にモードの切り替えに対応する。
 */
#define MODE0	(P12_bit.no1)
#define MODE1	(P12_bit.no2)

#define SCROLL_DELAY_MS	(200)	// スクロール遅延ミリ秒

static uint8_t get_mode(void);
static void brightness_max_and_all_light_demo(void);
static void controll_brightness_demo(void);
static void presentation_demo(void);
static void nop(void);

/**
 * 動作テーブル
 *
 * -----+----------------------
 * MODE |      動作モード
 * -----+----------------------
 *  00  | デモ1(輝度最大全点灯)
 *  01  | デモ2(輝度調整デモ)
 *  10  | デモ3(展示用デモ)
 *  11  | 外部制御モード
 * -----+----------------------
 */
typedef void (*action_t)(void);
static const action_t action_table[] = {
	brightness_max_and_all_light_demo,
	controll_brightness_demo,
	presentation_demo,
	nop,
};

static uint32_t demo_base_count;

static uint8_t cur_mode = 0;

static uint8_t data[NUM_OF_7SEG] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };
static uint8_t brightness[NUM_OF_7SEG] = { 100, 100, 100, 100, 100, 100, 100, 100, };
static const uint8_t num_to_pattern[] = {
	0xfc, /* 0 */
	0x60, /* 1 */
	0xda, /* 2 */
	0xf2, /* 3 */
	0x66, /* 4 */
	0xb6, /* 5 */
	0xbe, /* 6 */
	0xe4, /* 7 */
	0xfe, /* 8 */
	0xf6, /* 9 */
};

/************************************************************
 * 公開関数
 ************************************************************/

 /**
  * @par 処理内容
  * - モードを設定する。
  */
void mode_init(void)
{
	mode_update();
}

/**
 * @par 処理内容
 * - 現在のモード用のアクション関数を呼び出す。
 */
void mode_proc(void)
{
	demo_base_count++;
	action_table[cur_mode]();
}

/**
 * @par 処理内容
 * - モード端子を読み取る。
 * - 現在のモードと異なっていた場合、モードを更新する。
 * - 更新する場合、デモ用カウンタは初期化する。
 */
void mode_update(void)
{
	uint8_t new_mode = get_mode();
	if (cur_mode != new_mode) {
		cur_mode = new_mode;
		demo_base_count = 0;
	}
}

/************************************************************
 * 内部関数
 ************************************************************/

static uint8_t get_mode(void)
{
	return ((MODE1<<1) | MODE0);
}

static uint8_t banner[] = {
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	
	0xe4, /* 7 */
	0xb6, /* S */
	0x9e, /* E */
	0xbc, /* G */
	0x10, /* _ */
	0x8e, /* F */
	0x60, /* I */
	0xec, /* N */
	0xbc, /* G */
	0x9e, /* E */
	0xef, /* R */
	
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
};

/**
 * [8][8][8][8][8][8][8][8]
 */
static void brightness_max_and_all_light_demo(void)
{	
	memset(data, 0xff, sizeof(data));
	memset(brightness, 100, sizeof(brightness));
	
	light_set_data(data);
	light_set_brightness(brightness);
	light_update();
}

/**
 * [%][3][d][ ][8][8][8][8]
 *
 * 前半3桁で輝度(0-100)を表示、後半4桁で輝度設定。
 * 「0→100(カウントアップ)」→「100→0(カウントダウン)」を繰り返す。
 * 10ミリ秒毎に、1ずつカウントアップ/カウントダウンする。
 */
static void controll_brightness_demo(void)
{
	BOOL count_update_timing_flag;
	BOOL count_up_phase_flag;
	uint32_t brightness_value;
	
	// 輝度計算
	// 例1: 1234[ms] --> 23
	// 例2: 7890[ms] --> 89
	brightness_value = demo_base_count / 10 % 100;
	
	// カウント更新タイミングか
	// 例1: 1234[ms] --> 4 --> FALSE
	// 例2: 7890[ms] --> 0 --> TRUE
	count_update_timing_flag = ((demo_base_count % 10) == 0) ? TRUE : FALSE;
	if (!count_update_timing_flag) {
		return;
	}
	
	// カウントアップフェーズか
	// 起動時(0～1000[ms])はカウントアップから始まってほしいので、
	// 偶数秒→奇数秒をカウントアップとする。
	// 例1: 1234[ms] --> 1 --> 1 --> FALSE
	// 例2: 7890[ms] --> 7 --> 1 --> FALSE
	count_up_phase_flag = (((demo_base_count / 1000) % 2) == 0) ? TRUE : FALSE;
	if (!count_up_phase_flag) {
		brightness_value = 100 - brightness_value;
	}

	// "%3d"。先頭が0の場合は表示しない。
	data[0] = num_to_pattern[brightness_value / 100 % 10];
	if (data[0] == num_to_pattern[0]) {
		data[0] = 0x00;
	}
	data[1] = num_to_pattern[brightness_value / 10 % 10];
	if (data[1] == num_to_pattern[0]) {
		if (data[0] == 0x00) {
			data[1] = 0x00;
		}
	}
	data[2] = num_to_pattern[brightness_value / 1 % 10];
	data[3] = 0;
	data[4] = 0xff;
	data[5] = 0xff;
	data[6] = 0xff;
	data[7] = 0xff;
	
	brightness[0] = 100;
	brightness[1] = 100;
	brightness[2] = 100;
	brightness[3] = 100;
	brightness[4] = brightness_value;
	brightness[5] = brightness_value;
	brightness[6] = brightness_value;
	brightness[7] = brightness_value;
	
	light_set_data(data);
	light_set_brightness(brightness);
	light_update();
}

/**
 * [7][S][E][G][_][F][I][N][G][E][R]をスクロール表示
 */
static void presentation_demo(void)
{
	// TODO: 初期化関数を作成してモード変更の度に初期位置から始めたい。
	static int pos = 0;
	
	if (demo_base_count % SCROLL_DELAY_MS == 0) {
		pos++;
		if (pos == ((sizeof(banner) / sizeof(banner[0])) - 8 + 1)) {
			pos = 0;
		}
	}
	
	memcpy(data, &banner[pos], sizeof(data));
	memset(brightness, 100, sizeof(brightness));
	
	light_set_data(data);
	light_set_brightness(brightness);
	
	// 本来は外部からのLATCH信号で更新するが、
	// デモなので内部から更新する。
	light_update();
}

static void nop(void)
{
	/* 何もしない */
}
