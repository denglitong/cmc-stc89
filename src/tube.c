//
// Created by Litong Deng on 2022/10/8.
//

#include "tube.h"

#include <8051.h>

#include "common.h"

#define TUBE_SIZE 6

void enable_74hc138() {
  // 74HC138 芯片的使能引脚，G1 高电平 G2 低电平 才能启动74HC138的 3-8 译码电路
  ADDR_3 = 1;  // G1 高电平
  EN_LED = 0;  // G2低电平（G2A, G2B）
}

// i: 0 - (TUBE_SIZE-1)
void enable_tube(unsigned char i) {
  // P1_2 P1_1 P1_0
  // TUBE 0 000
  // TUBE 1 001
  // TUBE 2 010
  // TUBE 3 011
  // TUBE 4 100
  // TUBE 5 101
  P1 &= 1 << 3;
  P1 |= i;
}

void turn_on_all_segs() { P0 = 0x00; }

void turn_off_all_segs() { P0 = 0xff; }

unsigned char seg(unsigned char i) { return 0x01 << i; }

#define DOT 10

/**
 * P0 = 0xff ^ digit_seg(i) shows number i in digital tube
 *
 *        0
 *   ............
 *   .          .
 * 5 .          .   1
 *   .    6     .
 *   ............
 *   .          .
 * 4 .          .   2
 *   .          .
 *   ............
 *        3
 *                  .   7
 */
unsigned char digit_seg(unsigned char i) {
  switch (i) {
    case 0:
      return seg(0) | seg(5) | seg(1) | seg(4) | seg(3) | seg(2);
    case 1:
      return seg(1) | seg(2);
    case 2:
      return seg(0) | seg(1) | seg(6) | seg(4) | seg(3);
    case 3:
      return seg(0) | seg(1) | seg(2) | seg(3) | seg(6);
    case 4:
      return seg(5) | seg(6) | seg(1) | seg(2);
    case 5:
      return seg(0) | seg(5) | seg(6) | seg(2) | seg(3);
    case 6:
      return seg(0) | seg(5) | seg(6) | seg(2) | seg(3) | seg(4);
    case 7:
      return seg(0) | seg(1) | seg(2);
    case 8:
      return seg(0) | seg(5) | seg(6) | seg(1) | seg(4) | seg(3) | seg(2);
    case 9:
      return seg(0) | seg(5) | seg(6) | seg(1) | seg(3) | seg(2);
    case DOT:
      // show .
      return seg(7);
    default:
      return 0x00;
  }
}

void show_digit(unsigned char i) { P0 = 0xff ^ digit_seg(i); }

_Noreturn void turn_on_tube() {
  // 连通 74hc138 引脚使能 3-8 译码器输出以连通各个数码管
  enable_74hc138();

  unsigned char digit = 1, tube_idx = 0;
  while (1) {
    // 依次连通各个数码管，一次只能点亮一个数码管
    enable_tube(TUBE_SIZE - 1 - tube_idx++);
    // 在数码管上展示数字 digit
    show_digit(digit++);
    delay_ms(500);

    // 索引数字复位
    if (tube_idx >= TUBE_SIZE) {
      tube_idx = 0;
    }
    if (digit >= 10) {
      digit = 0;
    }
  }
}
