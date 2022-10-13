//
// Created by Litong Deng on 2022/10/4.
//

#include "led.h"

#include <8051.h>

#include "common.h"

// 教学板子 LED_SINGLE 总开关
void turn_on_master_switch() {
  // 74HC138 芯片，即 38 译码器，
  // LED 总开关对应的引脚为 74HC138 的输出口 LEDS6

  // 74HC138 芯片 的使能引脚，
  // G1 高电平 G2 低电平，才能启动 74HC138 芯片的 3-8 译码电路
  ADDR_3 = 1;  // G1 高电平
  EN_LED = 0;  // G2低电平（G2A, G2B）

  // 110 LEDS6 为低电平，三极管导通，LED 总开关打开
  ADDR_2 = 1;
  ADDR_1 = 1;
  ADDR_0 = 0;
}

_Noreturn void flash_single_led() {
  LED_SINGLE = P0_0;
  while (1) {
    // 高电平 - 低电平 点亮 LED_SINGLE
    LED_SINGLE = 0;
    delay_ms(200);
    // 高电平 - 高电平 熄灭 LED_SINGLE
    LED_SINGLE = 1;
    delay_ms(300);
  }
}

void turn_on_all_leds() { LED_LINE = 0x00; }

void turn_off_all_leds() { LED_LINE = 0xff; }

void turn_on_single_led(unsigned char i) {
  // turn on the i-th led from low-high
  LED_LINE ^= 0x01 << i;
}

_Noreturn void flash_serial_leds() {
  short i = 0;
  while (1) {
    for (i = 0; i < 2; ++i) {
      turn_on_all_leds();
      delay_ms(200);
      turn_off_all_leds();
      delay_ms(300);
    }
    for (i = 0; i < LED_SIZE; ++i) {
      // turn on the i-th led from low-high
      turn_on_single_led(i);
      delay_ms(100);
      turn_off_all_leds();
      delay_ms(100);
    }
    for (i = LED_SIZE - 1; i >= 0; --i) {
      // turn on the i-th led from high-low
      turn_on_single_led(i);
      delay_ms(100);
      turn_off_all_leds();
      delay_ms(100);
    }
  }
}

_Noreturn void turn_on_led() {
  turn_on_master_switch();
  // flash_single_led();
  flash_serial_leds();
}
