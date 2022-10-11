//
// Created by Litong Deng on 2022/10/11.
//

#include "led_matrix.h"

#include "common.h"
#include "tube.h"

extern unsigned long DIGIT;
extern unsigned int INTERRUPT_MILLIS;
extern unsigned char INTERRUPT_FLAG;
extern unsigned int INTERRUPT_COUNT;

const unsigned char MATRIX_ROW = 8;
const unsigned char MATRIX_COL = 8;

void enable_u4_74hc138() {
  // U4 74HC138 enable
  // E3 already H
  ADDR_3 = 0;  // set /E1 L, also disable U3 74HC138
  EN_LED = 0;  // set /E2 L
}

// row: 0 - 7
void enable_led_row(unsigned char row) {
  // C B A  => 0-7 => LEDC0- LEDC7
  P1 &= 1 << 3;
  P1 |= row;
}

// col: 0 - 7
void enable_led_column(unsigned char col) {
  // DB0-DB7 P0
  P0 = 0xff;
  P0 ^= 1 << col;
}

void turn_off_led_matrix() { EN_LED = 1; }

_Noreturn void turn_on_led_matrix_normal() {
  enable_u4_74hc138();
  while (1) {
    for (int i = 0; i < MATRIX_ROW; ++i) {
      enable_led_row(i);
      for (int j = 0; j < MATRIX_COL; ++j) {
        enable_led_column(j);
        delay_ms(200);
        turn_off_led_matrix();
        delay_ms(200);
      }
    }
  }
}

unsigned int digit_row() { return (DIGIT % 64) / 8; }

unsigned int digit_col() { return DIGIT % 64 - digit_row() * 8; }

void turn_on_led_matrix_show_digit_in_tube() {
  enable_u4_74hc138();
  enable_led_row(digit_row());
  enable_led_column(digit_col());

  enable_u3_74hc138();
  show_digit_on_all_tubes();
}

_Noreturn void turn_on_led_matrix_with_interrupt(
    unsigned int interrupt_millis) {
  EA = 1;   // enable global interrupt
  ET1 = 1;  // enable Timer1 interrupt

  // setup T1_M1 = 0, T1_M0 = 1 (Timer1 mode TH1-TL1 16 bits timer)
  TMOD = 0x10;
  // setup TH0 TL0 initial value
  TH1 = 0xFC;
  TL1 = 0x67;
  TR1 = 1;  // start/enable Timer1

  enable_u4_74hc138();
  INTERRUPT_MILLIS = interrupt_millis;

  while (1) {
    if (INTERRUPT_FLAG) {
      INTERRUPT_FLAG = 0;
      DIGIT++;
      if (DIGIT >= 999999) {
        DIGIT = 0;
      }
    }
  }
}

// 中断函数 函数原型需要加上 __interrupt(x)，x 对应不同中断
// 中断函数不需要调用，达到中断时自动进入
// 定时器 T1 产生溢出时触发一个 T1 中断，对应函数标号为 3
// NOTE: 中断函数需要在 header 文件中声明进行声明才能生效！
void InterruptTime1() __interrupt(3) {
  // setup TH0 TL0 initial value, each interrupt(Timer0 overflow) will pass 1ms
  TH1 = 0xFC;
  TL1 = 0x67;

  INTERRUPT_COUNT++;
  if (INTERRUPT_COUNT >= INTERRUPT_MILLIS) {  // 1ms * INTERRUPT_MILLIS
    INTERRUPT_COUNT = 0;
    INTERRUPT_FLAG = 1;
  }

  enable_led_row(digit_row());
  enable_led_column(digit_col());
}

_Noreturn void turn_on_led_matrix() {
  // turn_on_led_matrix_normal();
  turn_on_led_matrix_with_interrupt(100);
}
