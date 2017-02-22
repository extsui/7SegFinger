/**
 * @file light.h
 * @brief “_“”•”
 *
 * Copyright (C) 2017 extsui, All rights reserved.
 */

#ifndef LIGHT_H
#define LIGHT_H

void light_init(void);
void light_set_data(const uint8_t data[]);
void light_set_brightness(const uint8_t brightness[]);
void light_update(void);

#endif /* LIGHT_H */
