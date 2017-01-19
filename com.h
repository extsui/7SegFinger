/**
 * @file com.h
 * @brief ’ÊM•”
 *
 * Copyright (C) 2017 extsui, All rights reserved.
 */

#ifndef COM_H
#define COM_H

void com_init(void);

// TODO: WORKAROUND:
// ˆÈ‰º‚ÍAr_cg_*.c‚©‚çŠ„‚è‚İŠÖ”‚ğˆÚ“®‚·‚é‚±‚Æ‚ÅstaticŠÖ”‚É‚·‚é—\’èB
void com_receive_trigger_callback(void);
void com_received_callback(void);

#endif /* COM_H */
