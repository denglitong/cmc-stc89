//
// Created by Litong Deng on 2022/10/13.
//

#include "key.h"

#include "common.h"
#include "led.h"

unsigned char KEY_STATUS = 1;
unsigned char PREV_KEY_STATUS = 1;

_Noreturn void get_single_input_key_with_delay() {
  turn_on_led_master_switch();
  turn_off_all_leds();

  while (1) {
    KEY_OUT_1 = 0;

    // KeyIn1 状态发生变化
    if (PREV_KEY_STATUS != KEY_IN_1) {
      // 延时 10ms 以消除物理按键抖动
      // 【在正式项目里使用这种延时逻辑是不被允许的】
      delay_ms(10);
      // 按键消除抖动后 KeyIn1 如果仍然是 turn on 状态
      if (KEY_IN_1 == 0) {
        // 改变 LED0 的状态：亮->灭->亮->灭
        switch_single_led(0);
      }
      PREV_KEY_STATUS = KEY_IN_1;
    }
  }
}

_Noreturn void get_single_input_key_with_interrupt() {
  turn_on_led_master_switch();
  turn_off_all_leds();

  EA = 1;   // enable global interrupt
  ET0 = 1;  // enable Timer0 interrupt

  // setup T0_M1 = 0, T0_M0 = 1 (Timer0 mode TH0-TL0 16 bits timer)
  TMOD = 0x01;
  // setup TH0 TL0 initial value
  TH0 = 0xFC;
  TL0 = 0x67;
  TR0 = 1;  // start/enable Timer0

  while (1) {
    KEY_OUT_1 = 0;

    if (PREV_KEY_STATUS != KEY_STATUS) {
      // 按键消除抖动后 KeyIn1 如果仍然是 turn on 状态
      if (KEY_STATUS == 0) {
        // 改变 LED0 的状态：亮->灭->亮->灭
        switch_single_led(0);
      }
      PREV_KEY_STATUS = KEY_STATUS;
    }
  }
}

void InterruptTime0_key() __interrupt(1) {
  // setup TH0 TL0 initial value, each interrupt(Timer0 overflow) will pass 1ms
  TH0 = 0xFC;
  TL0 = 0x67;

  // 2bytes, 16bits, 16ms
  static unsigned short key_buff = 0xFFFF;

  key_buff = (key_buff << 1) | KEY_IN_1;
  if (key_buff == 0x0000) {
    // 单片机往往是检测状态，而不是监测过程；常常利用中断来判断按键处于什么状态
    KEY_STATUS = 0;
  } else if (key_buff == 0xFFFF) {
    KEY_STATUS = 1;
  }
}

_Noreturn void show_key_input() {
  // get_single_input_key_with_delay();
  get_single_input_key_with_interrupt();
}
