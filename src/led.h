//
// Created by Litong Deng on 2022/10/4.
//

#ifndef CMC_STC89_LED_H
#define CMC_STC89_LED_H

void turn_on_led_master_switch();

void turn_on_all_leds();

void turn_off_all_leds();

void turn_on_single_led(unsigned char i);

void turn_off_single_led(unsigned char i);

void switch_single_led(unsigned char i);

_Noreturn void turn_on_led();

#endif  // CMC_STC89_LED_H
