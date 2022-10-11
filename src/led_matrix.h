//
// Created by Litong Deng on 2022/10/11.
//

#ifndef MCU_STC89_LED_MATRIX_H
#define MCU_STC89_LED_MATRIX_H

#include <8051.h>

_Noreturn void turn_on_led_matrix();

void InterruptTime1() __interrupt(3);

#endif  // MCU_STC89_LED_MATRIX_H
