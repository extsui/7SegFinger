/**
 * @file com.h
 * @brief �ʐM��
 *
 * Copyright (C) 2017 extsui, All rights reserved.
 */

#ifndef COM_H
#define COM_H

void com_init(void);

// TODO: WORKAROUND:
// �ȉ��́Ar_cg_*.c���犄�荞�݊֐����ړ����邱�Ƃ�static�֐��ɂ���\��B
void com_receive_trigger_callback(void);
void com_received_callback(void);

#endif /* COM_H */
