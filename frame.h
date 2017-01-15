/**
 * @file frame.h
 * @brief ƒtƒŒ[ƒ€•”
 *
 * Copyright (C) 2017 extsui, All rights reserved.
 */

#ifndef FRAME_H
#define FRAME_H

extern const int g_frame_size;

void frame_init(void);
uint8_t frame_analyze_proc(const uint8_t data[]);

#endif /* FRAME_H */
