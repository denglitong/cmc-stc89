//
// Created by Litong Deng on 2022/10/8.
//  数码管
//

#ifndef MCU_STC89_TUBE_H
#define MCU_STC89_TUBE_H

#include <8051.h>

_Noreturn void turn_on_tube();

void enable_tube(unsigned char i);

void turn_on_all_segs();

void turn_off_all_segs();

unsigned char seg(unsigned char i);

unsigned char digit_seg(unsigned char i);

void show_digit(unsigned char i);

void show_digit_on_all_tubes();

unsigned int pow(unsigned int x, unsigned int y);

_Noreturn void turn_on_tube_with_func_pointer();

_Noreturn void turn_on_tube_with_interrupt(unsigned int interrupt_millis);

void InterruptTime0() __interrupt(1);

#endif  // MCU_STC89_TUBE_H
