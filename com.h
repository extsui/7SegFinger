/**
 * @file com.h
 * @brief 通信部
 *
 * Copyright (C) 2017 extsui, All rights reserved.
 */

#ifndef COM_H
#define COM_H

void com_init(void);

// TODO: WORKAROUND:
// 以下は、r_cg_*.cから割り込み関数を移動することでstatic関数にする予定。
void com_receive_trigger_callback(void);
void com_received_callback(void);
void com_update_callback(void);

#endif /* COM_H */
