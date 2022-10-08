//
// Created by Litong Deng on 2022/10/4.
//

#include "common.h"

#include <8051.h>

void delay_ms(unsigned int s) {
  // delay_ms_imprecise(s);
  delay_ms_precise(s);
}

void delay_ms_imprecise(unsigned int s) {
  unsigned int x;
  for (s; s > 0; s--) {
    // 这里 x 设置为 98 猜测是和晶振电路的频率有关
    x = 98;
    while (x--)
      ;
  }
}

/**
 * 时钟晶振频率：11059200 (11.0592M)
 * 1 机器周期 = 12 时钟周期
 * 1ms 所需机器周期 = 11059200 * 0.001 / 12 = 921
 * 需要让计数器完成 921 次机器周期，以取得 1ms 的系统时钟
 * TH0 TL0 16bits 计数范围 2^16 = 65536
 * 设置 TH0 TL0 初始位位 65536-921=0xFC67，
 * 则当 TL0 溢出时即完成了一次 0xFC67~65536 的机器周期循环，即是 1ms
 * 所以 TH0 TL0 初始位设置为 TH0=0xFC TL0=0x67
 * 判断 TL0 是否使用 TF0 寄存器位
 * @param s
 */
void delay_ms_precise(unsigned int s) {
  // setup T0_M1 = 0, T0_M0 = 1 (Timer0 mode TH0-TL0 16 bits timer)
  TMOD = 0x01;

  // setup TH0 TL0 initial value
  TH0 = 0xFC;
  TL0 = 0x67;

  // start/enable Timer0
  TR0 = 1;

  // delay s milliseconds
  while (s--) {
    while (1) {
      // if Timer0 overflow (has passed 1ms)
      if (TF0) {
        // 软件复位
        TF0 = 0;
        TH0 = 0xFC;
        TL0 = 0x67;
        break;
      }
    }
  }

  // stop/disable Timer0
  TR0 = 0;
}
