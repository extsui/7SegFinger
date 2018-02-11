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

//#define SCROLL_DELAY_MS	(200)	// スクロール遅延ミリ秒
#define SCROLL_DELAY_MS	(100)	// スクロール遅延ミリ秒

static uint8_t get_mode(void);
static void nop(void);
static void update(void);

static void brightness_max_and_turn_on_all(void);
static void brightness_min_and_turn_off_all(void);
static void controll_brightness_demo(void);
static void presentation_demo_init(void);
static void presentation_demo(void);

typedef void (*action_t)(void);

/**
 * 状態用のアクション構造体
 */
typedef struct {
	action_t entry;	/**< 状態に入った時のアクション */
	action_t proc;	/**< 状態の中で1ms毎に呼び出されるアクション */
} state_action_t;

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
static const state_action_t state_table[] = {
	{ brightness_max_and_turn_on_all,	nop,						},
	{ nop,								controll_brightness_demo,	},
	{ presentation_demo_init,			presentation_demo,			},
	{ brightness_min_and_turn_off_all,	nop,						},
};

static uint32_t demo_base_count = 0;

/**
 * 現在のモード
 * 
 * 起動時に初期化処理が必ず実行されるようにするため、
 * モード端子であり得ない値(0-3以外)を設定しておく。
 */
static uint8_t cur_mode = 0xFF;

static uint8_t data[NUM_OF_7SEG] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };
static uint8_t brightness[NUM_OF_7SEG] = { 255, 255, 255, 255, 255, 255, 255, 255, };
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
	state_table[cur_mode].proc();
	demo_base_count++;
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
		state_table[cur_mode].entry();
	}
}

/************************************************************
 * 内部関数
 ************************************************************/

static uint8_t get_mode(void)
{
	uint8_t mode = ((MODE1<<1) | MODE0);
	return mode;
}

static void nop(void)
{
	/* 何もしない */
}

/**
 * 現状のデータと輝度で表示を更新する。
 */
static void update(void)
{
	light_set_data(data);
	light_set_brightness(brightness);
	
	// 本来は外部からのLATCH信号で更新するが、
	// デモなので内部から更新する。
	light_update();
}

/************************************************************/

/**
 * [8][8][8][8][8][8][8][8]
 */
static void brightness_max_and_turn_on_all(void)
{
	memset(data, 0xff, sizeof(data));
	memset(brightness, 255, sizeof(brightness));
	update();
}

/**
 * [ ][ ][ ][ ][ ][ ][ ][ ]
 */
static void brightness_min_and_turn_off_all(void)
{
	memset(data, 0x00, sizeof(data));
	memset(brightness, 0, sizeof(brightness));
	update();
}

/************************************************************/

/**
 * [%][3][d][ ][8][8][8][8]
 *
 * 前半3桁で輝度(0-255)を表示、後半4桁で輝度設定。
 * 「0→255(カウントアップ)」→「255→0(カウントダウン)」を繰り返す。
 * 10ミリ秒毎に、1ずつカウントアップ/カウントダウンする。
 */
static void controll_brightness_demo(void)
{
	BOOL count_update_timing_flag;
	BOOL count_up_phase_flag;
	uint32_t brightness_value;
	
	// 輝度計算
	// カウンタの10msの桁を1の位として、下位8ビットを輝度として使用する。
	brightness_value = (demo_base_count / 10) & 0xFF;
	
	// カウント更新タイミングかz
	// 例1: 1234[ms] --> 4 --> FALSE
	// 例2: 7890[ms] --> 0 --> TRUE
	count_update_timing_flag = ((demo_base_count % 10) == 0) ? TRUE : FALSE;
	if (!count_update_timing_flag) {
		return;
	}
	
	// カウントアップフェーズか
	// 起動時はカウントアップから始まってほしいので、
	// 偶数秒→奇数秒をカウントアップとする。
	count_up_phase_flag = ((((demo_base_count / 10) >> 8) & 0x1) == 0) ? TRUE : FALSE;
	if (!count_up_phase_flag) {
		brightness_value = 255 - brightness_value;
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
	
	brightness[0] = 255;
	brightness[1] = 255;
	brightness[2] = 255;
	brightness[3] = 255;
	brightness[4] = brightness_value;
	brightness[5] = brightness_value;
	brightness[6] = brightness_value;
	brightness[7] = brightness_value;
	
	update();
}

/************************************************************/

/** 展示用デモのバナー */
//static const uint8_t banner[] = {
/*	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
*/

//	0x40,
//	0xe4, /* 7 */
//	0x4d,
//	0xb6, /* S */
//	0x69,
//	0x9e, /* E */
//	0x61,
//	0xbc, /* G */
//	0x09,
//	0x10, /* _ */
//	0x60,	
//	0x8e, /* F */
//	0x00,
//	0x60, /* I */
//	0x6c,
//	0xec, /* N */
//	0x6d,
//	0xbc, /* G */
//	0x69,
//	0x9e, /* E */
//	0x60,
//	0xef, /* R */
//	0x0c,
/*	
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
};
*/

static const uint8_t banner[] = {
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

static const uint8_t banner_virtual[] = {
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,

//	0x40,
//	0x0c,
	
	0x40, /*  +7 */
	0x4d, /* 7+S */
	0x69, /* S+E */
	0x61, /* E+G */
	0x09, /* G+_ */
	0x60, /* _+F */
	0x00, /* F+I */
	0x6c, /* I+N */
	0x6d, /* N+G */
	0x69, /* G+E */
	0x60, /* E+R */
	0x0c, /* R+  */

	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
};


/** スクロール位置(展示用デモで使用) */
static int scroll_pos;

/** スクロールタイミング */
static uint32_t next_scroll_time;

static void presentation_demo_init(void)
{
	scroll_pos = 0;
	next_scroll_time = demo_base_count + SCROLL_DELAY_MS;
}

/**
 * [7][S][E][G][_][F][I][N][G][E][R]をスクロール表示
 */
static void presentation_demo(void)
{
	if (demo_base_count >= next_scroll_time) {
		static BOOL virtual_timing = FALSE;
	
		next_scroll_time += SCROLL_DELAY_MS;
		
		if (!virtual_timing) {
			memcpy(data, &banner[scroll_pos], sizeof(data));
			virtual_timing = TRUE;
		} else {
			scroll_pos++;
			memcpy(data, &banner_virtual[scroll_pos], sizeof(data));
			virtual_timing = FALSE;
		}
		
		if (scroll_pos == ((sizeof(banner) / sizeof(banner[0])) - 8 + 1)) {
			scroll_pos = 0;
		}
	}
	
	memset(brightness, 255, sizeof(brightness));
	
	update();
}
