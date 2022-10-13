//
// Created by Litong Deng on 2022/10/13.
//

#ifndef MCU_STC89_KEY_H
#define MCU_STC89_KEY_H

#include <8051.h>

_Noreturn void show_key_input();

void InterruptTime0_key() __interrupt(1);

#endif  // MCU_STC89_KEY_H
