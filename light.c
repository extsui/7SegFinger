/**
 * @file light.c
 * @brief �_�����̎���
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

/** �_���p�f�[�^�\���� */
typedef struct {
	uint8_t data;		///< �\���f�[�^
	uint8_t brightness;	///< �P�x(0-255)
} light_t;

/** �_�����f�[�^��� */
static light_t light[NUM_OF_7SEG];

/** �X�V�p�f�[�^��� */
static light_t latch[NUM_OF_7SEG];

/** ���݂̓_���ӏ� */
static int light_cur_pos;

/*
 * �_�C�i�~�b�N�_������̃^�C�~���O�`���[�g
 *
 * ���_��7�Z�O�ʒu
 * [0]                  [1]                  [2]
 * <-------------------><-------------------><---...
 *  |
 *  +-----�_���}�X�^����(1000�`50000us)
 *     +--PWM�ő����(�_���}�X�^���� - �����ԁ�)
 *     |
 *    <---------------->
 * <-><-----------><---><-><-----------><---><-><...
 *  |  |            |   
 *  |  |            +---PWM OFF����
 *  |  +----------------PWM ON����
 *  +-------------------74HC595�ւ̓]�����ԓ�(PWM��OFF)
 *
 * �����̊��Ԃ������Ə������Ԃ������ď���P�x�������邽��
 *   �Ȃ�ׂ��Z���������B�������A�Z�����߂���ƃf�[�^�w���
 *   �A�����čs�����ꍇ�Ƀ`��������������B
 *   ���100us�����肪�ǂ�(50us���ƃ`�����L��)�B
 */
// ��L�̐}�́u�����ԁv�̒l�B
// �Z���ĊԂɍ����Ȃ�Z�������ǂ��B
// 100us@(CLK=20MHz, DIV=16) ---> 250
// 50us                      ---> 125
#define LIGHT_PWM_BLANK_VALUE	(250)
 
/** �_���}�X�^������TDR�l(�_�C�i�~�b�N�_�������̑匳) */
static uint16_t light_master_cycle_value;

/** PWM�ő������TDR�l */
static uint16_t light_pwm_width_value;

static void set_pwm_duty(uint8_t duty);

/**
 * �_�����̏�����
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
	R_TAU0_Channel0_Start();	// �P�x�p�^�C�}
	R_TAU0_Channel2_Start();	// �\���X�V�p�^�C�}
}

/**
 * �\���f�[�^�̐ݒ�
 */
void light_set_data(const uint8_t data[])
{
	int i;
	for (i = 0; i < NUM_OF_7SEG; i++) {
		latch[i].data = data[i];
	}
}

/**
 * �P�x�̐ݒ�
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
	// �����`�F�b�N
	if ((light_cycle_us < 1000 ) || (50000 < light_cycle_us)) {
		return -1;
	}
	
	// �ʕb����J�E���^�l�ւ̕ϊ�
	// �E1�J�E���g������̃ʕb = CLOCK_HZ / TAU0_CHANNEL2_DIVISOR / 1000 / 1000 [us/cnt]
	//   �����ۂ̌v�Z�́A���x�̊֌W�ŏ��Ԃ�ς��Ă��邱�Ƃɒ��ӁB
	// �ECLOCK_HZ=20MHz�ADIVISOR=16 �̏ꍇ�́A1.25[us/cnt]�B
	light_master_cycle_value = (uint16_t)((((CLOCK_HZ / 1000) / TAU0_CHANNEL2_DIVISOR) * (uint32_t)light_cycle_us) / 1000);
	light_pwm_width_value = light_master_cycle_value - LIGHT_PWM_BLANK_VALUE;
	
	return 0;
}

/**
 * �\�����f�[�^��ݒ肵���f�[�^�ɍX�V����
 */
void light_update(void)
{
	memcpy(light, latch, sizeof(light));
}

/**
 * �\���؂�ւ��R�[���o�b�N
 *
 * ���ݓ_�����Ă���7�Z�O��؂�ւ���
 */
void light_move_to_next_pos_callback(void)
{
	static uint8_t shift_data[sizeof(light_t)];
	static const uint8_t bit_table[] = {
		0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01,
	};
	
	shift_data[0] = bit_table[light_cur_pos];
	shift_data[1] = light[light_cur_pos].data;
	
	// �uCSI01�J�n�`�������荞�݁v�̎��Ԃɂ��āA
	// 1MHz�̏ꍇ�͖�20us�A5MHz�̏ꍇ�͖�10us�ƂȂ����B(@20MHz)
	// �ˊ��荞�ݓP�p�ɂ���6us�ƂȂ������߁A������̗p����B
	R_CSI01_SendBlocking(shift_data, sizeof(shift_data));
	
	set_pwm_duty(light[light_cur_pos].brightness);

	// �_�������X�V
	TDR02H = (uint8_t)((light_master_cycle_value & 0xFF00) >> 8);
	TDR02L = (uint8_t)((light_master_cycle_value & 0x00FF) >> 0);
	
	// RCK�͗����オ��Ŕ��f
	PIN_RCK = 1;
	PIN_RCK = 0;
	
	// ���Z���g�p���Ă��邪�A���߃��x���ł̓V�t�g���Z��
	// �u���������Ă���A���ʕb����������Ȃ��B
	light_cur_pos = (light_cur_pos + 1) % NUM_OF_7SEG;
}

/**
 * PWM�o�͂̃f���[�e�B��(0-255)��ݒ肷��B
 *
 * �����V���b�g�E�p���X�@�\���g�p���Ď�������B
 */
static void set_pwm_duty(uint8_t duty)
{
	uint8_t new_tdr01h, new_tdr01l;
	uint16_t tdr_duty_255 = light_pwm_width_value;
	uint16_t tdr_duty_x = (uint16_t)((uint32_t)tdr_duty_255 * duty >> 8);
	
	// TDR0nH��TDR0nL�̏��ԂɘA���ŏ������ޕK�v������
	new_tdr01h = (uint8_t)((tdr_duty_x & 0xFF00) >> 8);
	new_tdr01l = (uint8_t)(tdr_duty_x & 0x00FF);

	// �����V���b�g�}�X�^�����̐ݒ�(�ŏ��l�BS/W�g���K�p)
	TDR00H = 0;	// �ŏ��l�Œ�(PWM����J�n�܂ł̎���)
	TDR00L = 0;	// �ŏ��l�Œ�(PWM����J�n�܂ł̎���)
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
	// PWM�o�͊����^�C�~���O
	// �����Ŏ��̎�����PWM�f���[�e�B���m�肵��TDR�ɐݒ肵�Ă����B
	// �������Ԃ���17us�Ȃ̂Ńt���[���𗎂Ƃ��Ȃ��悤�ɂ��邽�߂ɂ́A
	// SPI��M1�o�C�g�ɂ����鏈�����Ԃ����̒l�ȏ�ł���K�v������B
	// ��SPI��M�N���b�N400kHz�̏ꍇ1�o�C�g��M��20us�Ȃ̂ŁA
	//   �����܂Ŏ��g���������Ă��΃t���[�����X�͖����Ȃ�B
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
