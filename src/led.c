//
// Created by Litong Deng on 2022/10/4.
//

#include "led.h"

#include <8051.h>

#include "common.h"

#define LED_SIZE 8
#define LED_LINE P0
#define LED_SINGLE P0_0

#define ADDR_0 P1_0
#define ADDR_1 P1_1
#define ADDR_2 P1_2
#define ADDR_3 P1_3
#define EN_LED P1_4

void turn_on_master_switch() {
  // 教学板子 LED_SINGLE 总开关
  ADDR_0 = 0;
  ADDR_1 = 1;
  ADDR_2 = 1;
  ADDR_3 = 1;
  EN_LED = 0;
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

_Noreturn void flash_serial_leds() {
  // initialize leds, all turn off
  LED_LINE = 0xff;
  while (1) {
    for (int i = 0; i < LED_SIZE; ++i) {
      // turn on the i-th led from low-high
      LED_LINE ^= 0x01 << i;
      delay_ms(200);
      // turn off
      LED_LINE = 0xff;
      delay_ms(300);
    }
    for (int i = LED_SIZE - 1; i >= 0; --i) {
      // turn on the i-th led from high-low
      LED_LINE ^= 0x01 << i;
      delay_ms(200);
      // turn off
      LED_LINE = 0xff;
      delay_ms(300);
    }
  }
}

_Noreturn void turn_on_led() {
  turn_on_master_switch();
  // flash_single_led();
  flash_serial_leds();
}
