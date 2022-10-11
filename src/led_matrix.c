//
// Created by Litong Deng on 2022/10/11.
//

#include "led_matrix.h"

#include "common.h"

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

void turn_off_led_matrix() { P0 = 0xff; }

_Noreturn void turn_on_led_matrix() {
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
