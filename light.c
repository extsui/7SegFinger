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

#pragma interrupt r_csi01_interrupt(vect=INTCSI01)
#pragma interrupt r_tau0_channel1_interrupt(vect=INTTM01)
#pragma interrupt r_tau0_channel2_interrupt(vect=INTTM02)

static void light_move_to_next_pos_callback(void);

#define PIN_nSCLR	(P0_bit.no3)
#define PIN_RCK		(P0_bit.no6)

/** 点灯用データ構造体 */
typedef struct {
	uint8_t data;		///< 表示データ
	uint8_t brightness;	///< 輝度(0-255)
} light_t;

/** 点灯中データ情報 */
static light_t light[NUM_OF_7SEG];

/** 更新用データ情報 */
static light_t latch[NUM_OF_7SEG];

/** 現在の点灯箇所 */
static int light_cur_pos;

/*
 * ダイナミック点灯制御のタイミングチャート
 *
 * ↓点灯7セグ位置
 * [0]                  [1]                  [2]
 * <-------------------><-------------------><---...
 *  |
 *  +-----点灯マスタ周期(1000～50000us)
 *     +--PWM最大周期(点灯マスタ周期 - 一定期間※)
 *     |
 *    <---------------->
 * <-><-----------><---><-><-----------><---><-><...
 *  |  |            |   
 *  |  |            +---PWM OFF期間
 *  |  +----------------PWM ON期間
 *  +-------------------74HC595への転送時間等(PWMはOFF)
 *
 * ※この期間が長いと消灯時間が増えて上限輝度が下がるため
 *   なるべく短くしたい。しかし、短くし過ぎるとデータ指定を
 *   連続して行った場合にチラつきが発生する。
 *   大体100us当たりが良い(50usだとチラつき有り)。
 */
// 上記の図の「一定期間」の値。
// 短くて間に合うなら短い方が良い。
// 100us@(CLK=20MHz, DIV=16) ---> 250
// 50us                      ---> 125
#define LIGHT_PWM_BLANK_VALUE	(250)
 
/** 点灯マスタ周期のTDR値(ダイナミック点灯周期の大元) */
static uint16_t light_master_cycle_value;

/** PWM最大周期のTDR値 */
static uint16_t light_pwm_width_value;

static void set_pwm_duty(uint8_t duty);

/**
 * 点灯部の初期化
 */
void light_init(void)
{
	PIN_nSCLR = 0;
	PIN_nSCLR = 1;
	PIN_RCK = 0;
	
	light_set_light_cycle(2000);
	
	memset(light, 0, sizeof(light));
	memset(latch, 0, sizeof(latch));
	
	light_cur_pos = 0;
	
	R_CSI01_Start();
	R_TAU0_Channel0_Start();	// 輝度用タイマ
	R_TAU0_Channel2_Start();	// 表示更新用タイマ
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

int light_set_light_cycle(uint16_t light_cycle_us)
{
	// 引数チェック
	if ((light_cycle_us < 1000 ) || (50000 < light_cycle_us)) {
		return -1;
	}
	
	// μ秒からカウンタ値への変換
	// ・1カウント当たりのμ秒 = CLOCK_HZ / TAU0_CHANNEL2_DIVISOR / 1000 / 1000 [us/cnt]
	//   ※実際の計算は、精度の関係で順番を変えていることに注意。
	// ・CLOCK_HZ=20MHz、DIVISOR=16 の場合は、1.25[us/cnt]。
	light_master_cycle_value = (uint16_t)((((CLOCK_HZ / 1000) / TAU0_CHANNEL2_DIVISOR) * (uint32_t)light_cycle_us) / 1000);
	light_pwm_width_value = light_master_cycle_value - LIGHT_PWM_BLANK_VALUE;
	
	return 0;
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
	
	// 「CSI01開始～完了割り込み」の時間について、
	// 1MHzの場合は約20us、5MHzの場合は約10usとなった。(@20MHz)
	// ⇒割り込み撤廃により約6usとなったため、これを採用する。
	R_CSI01_SendBlocking(shift_data, sizeof(shift_data));
	
	set_pwm_duty(light[light_cur_pos].brightness);

	// 点灯周期更新
	TDR02H = (uint8_t)((light_master_cycle_value & 0xFF00) >> 8);
	TDR02L = (uint8_t)((light_master_cycle_value & 0x00FF) >> 0);
	
	// RCKは立ち上がりで反映
	PIN_RCK = 1;
	PIN_RCK = 0;
	
	// 除算を使用しているが、命令レベルではシフト演算に
	// 置き換えられており、数μ秒しかかからない。
	light_cur_pos = (light_cur_pos + 1) % NUM_OF_7SEG;
}

/**
 * PWM出力のデューティ比(0-255)を設定する。
 *
 * ワンショット・パルス機能を使用して実現する。
 */
static void set_pwm_duty(uint8_t duty)
{
	uint8_t new_tdr01h, new_tdr01l;
	uint16_t tdr_duty_255 = light_pwm_width_value;
	uint16_t tdr_duty_x = (uint16_t)((uint32_t)tdr_duty_255 * duty >> 8);
	
	// TDR0nH→TDR0nLの順番に連続で書き込む必要がある
	new_tdr01h = (uint8_t)((tdr_duty_x & 0xFF00) >> 8);
	new_tdr01l = (uint8_t)(tdr_duty_x & 0x00FF);

	// ワンショットマスタ周期の設定(最小値。S/Wトリガ用)
	TDR00H = 0;	// 最小値固定(PWM制御開始までの時間)
	TDR00L = 0;	// 最小値固定(PWM制御開始までの時間)
	TDR01H = new_tdr01h;
	TDR01L = new_tdr01l;
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel1_interrupt
* Description  : This function INTTM01 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_tau0_channel1_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
	// PWM出力完了タイミング
	// ここで次の周期とPWMデューティを確定してTDRに設定しておく。
	// 処理時間が約17usなのでフレームを落とさないようにするためには、
	// SPI受信1バイトにかかる処理時間がこの値以上である必要がある。
	// ⇒SPI受信クロック400kHzの場合1バイト受信が20usなので、
	//   ここまで周波数を下げてやればフレームロスは無くなる。
	//DEBUG_PIN = 1;
	light_move_to_next_pos_callback();
	//DEBUG_PIN = 0;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel2_interrupt
* Description  : This function INTTM02 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_tau0_channel2_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
	R_TAU0_Channel0_Set_SoftwareTriggerOn();
    /* End user code. Do not edit comment generated here */
}
