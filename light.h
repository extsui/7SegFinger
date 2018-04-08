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

/**
 * �_�������ݒ�
 * @param light_cycle_us �_������(us)�B�͈́F1000�`50000�B
 * @return 0�F���� / -1�F���s
 */
int light_set_light_cycle(uint16_t light_cycle_us);

void light_update(void);

#endif /* LIGHT_H */
