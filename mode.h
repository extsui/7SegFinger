/**
 * @file mode.h
 * @brief モード部
 *
 * Copyright (C) 2017 extsui, All rights reserved.
 */

/**
 * モード部の初期化
 */
void mode_init(void);

/**
 * モード処理
 *
 * 1ms毎に呼び出すこと。
 */
void mode_proc(void);

/**
 * モード更新
 *
 * 100ms毎に呼び出すこと。
 */
void mode_update(void);
