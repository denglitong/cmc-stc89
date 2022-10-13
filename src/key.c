//
// Created by Litong Deng on 2022/10/13.
//

#include "key.h"

#include "common.h"
#include "led.h"

_Noreturn void get_input_key_switch_led() {
  turn_on_led_master_switch();
  turn_off_all_leds();

  unsigned char PREV_KEY_IN_1 = 1;

  while (1) {
    KEY_OUT_1 = 0;

    // KeyIn1 状态发生变化
    if (PREV_KEY_IN_1 != KEY_IN_1) {
      // 延时 10ms 以消除物理按键抖动
      delay_ms(10);
      // 按键消除抖动后 KeyIn1 如果仍然是 turn on 状态
      if (KEY_IN_1 == 0) {
        // 改变 LED0 的状态：亮->灭->亮->灭
        switch_single_led(0);
      }
      PREV_KEY_IN_1 = KEY_IN_1;
    }
  }
}

_Noreturn void show_key_input() { get_input_key_switch_led(); }
