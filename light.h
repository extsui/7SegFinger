/**
 * @file light.h
 * @brief 点灯部
 *
 * Copyright (C) 2017 extsui, All rights reserved.
 */

#ifndef LIGHT_H
#define LIGHT_H

void light_init(void);
void light_set_data(const uint8_t data[]);
void light_set_brightness(const uint8_t brightness[]);

/**
 * 点灯周期設定
 * @param light_cycle_us 点灯周期(us)。範囲：1000～50000。
 * @return 0：成功 / -1：失敗
 */
int light_set_light_cycle(uint16_t light_cycle_us);

void light_update(void);

#endif /* LIGHT_H */
