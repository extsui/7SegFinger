/**
 * @file light.h
 * @brief �_����
 *
 * Copyright (C) 2017 extsui, All rights reserved.
 */

#ifndef LIGHT_H
#define LIGHT_H

void light_init(void);
void light_set_data(const uint8_t data[]);
void light_set_brightness(const uint8_t brightness[]);

// �ȉ��̊֐���r_cg_*.c�ȊO����Ăяo���Ȃ����ƁI
void light_next_pos_callback(void);
void light_update_callback(void);

#endif /* LIGHT_H */
