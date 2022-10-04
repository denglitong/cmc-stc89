//
// Created by Litong Deng on 2022/10/4.
//

#include "led.h"

#include <8051.h>

#include "common.h"

// 第一个 LED
#define LED P0_0

#define ADDR_0 P1_0
#define ADDR_1 P1_1
#define ADDR_2 P1_2
#define ADDR_3 P1_3
#define EN_LED P1_4

_Noreturn void turn_on_led() {
  // 教学板子 LED 总开关
  ADDR_0 = 0;
  ADDR_1 = 1;
  ADDR_2 = 1;
  ADDR_3 = 1;
  EN_LED = 0;

  while (1) {
    LED = 0;  // 高电平 - P0_0(低电平) 点亮 LED
    delay_ms(200);
    LED = 1;  // 高电平 - P0_0(高电平) 熄灭 LED
    delay_ms(400);
  }
}
