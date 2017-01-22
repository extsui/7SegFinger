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
void light_update(void);

// �ȉ��̊֐���r_cg_*.c�ȊO����Ăяo���Ȃ����ƁI
void light_move_to_next_pos_callback(void);
void light_update_shift_register_callback(void);

#endif /* LIGHT_H */
