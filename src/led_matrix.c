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

unsigned char HEART_IMAGE_IDX = 0;
unsigned char DIGIT_IMAGE_IDX = 0;
unsigned short SHOW_IMAGE_ROUND = 0;

const unsigned char HEART_IMAGE[] = {0b00000000, 0b01100110, 0b11111111,
                                     0b11111111, 0b01111110, 0b00111100,
                                     0b00011000, 0b00000000};

const unsigned char IMAGES_UP_BOTTOM[][8] = {
    // letter I
    {0b00000000, 0b00111100, 0b00011000, 0b00011000, 0b00011000, 0b00011000,
     0b00011000, 0b00111100},
    // heart chart ❤️
    {0b00000000, 0b01100110, 0b11111111, 0b11111111, 0b01111110, 0b00111100,
     0b00011000, 0b00000000},
    // letter U
    {0b00000000, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b01100110,
     0b01100110, 0b00111110},
    // digit charts 1234567890
    // {0b00000000, 0b00011000, 0b00011000, 0b00111000, 0b00011000, 0b00011000,
    //  0b00011000, 0b01111110},
    // {0b00000000, 0b00111100, 0b01100110, 0b00000110, 0b00001100, 0b00110000,
    //  0b01100000, 0b01111110},
    // {0b00000000, 0b00111100, 0b01100110, 0b00000110, 0b00011100, 0b00000110,
    //  0b01100110, 0b00111100},
    // {0b00000000, 0b00001100, 0b00011100, 0b00101100, 0b01001100, 0b01111110,
    //  0b00001100, 0b00001100},
    // {0b00000000, 0b01111110, 0b01100000, 0b01111100, 0b00000110, 0b00000110,
    //  0b01100110, 0b00111100},
    // {0b00000000, 0b00111100, 0b01100110, 0b01100000, 0b01111100, 0b01100110,
    //  0b01100110, 0b00111100},
    // {0b00000000, 0b01111110, 0b01100110, 0b00001100, 0b00001100, 0b00011000,
    //  0b00011000, 0b00011000},
    // {0b00000000, 0b00111100, 0b01100110, 0b01100110, 0b00111100, 0b01100110,
    //  0b01100110, 0b00111100},
    // {0b00000000, 0b00111100, 0b01100110, 0b01100110, 0b00111110, 0b00000110,
    //  0b01100110, 0b00111100},
    // {0b00000000, 0b00111100, 0b01100110, 0b01101110, 0b01110110, 0b01100110,
    //  0b01100110, 0b00111100},
    // letter charts A-Z
    // {0b00000000, 0b00111100, 0b01100110, 0b01100110, 0b01111110, 0b01100110,
    //  0b01100110, 0b01100110},
    // {0b00000000, 0b01111100, 0b01100110, 0b01100110, 0b01111100, 0b01100110,
    //  0b01100110, 0b01111100},
    // {0b00000000, 0b00111100, 0b01100110, 0b01100000, 0b01100000, 0b01100000,
    //  0b01100110, 0b00111100},
    // {0b00000000, 0b01111100, 0b01100110, 0b01100110, 0b01100110, 0b01100110,
    //  0b01100110, 0b01111100},
    // {0b00000000, 0b01111110, 0b01100000, 0b01100000, 0b01111100, 0b01100000,
    //  0b01100000, 0b01111110},
    // {0b00000000, 0b01111110, 0b01100000, 0b01100000, 0b01111100, 0b01100000,
    //  0b01100000, 0b01100000},
    // {0b00000000, 0b00111100, 0b01100110, 0b01100000, 0b01100000, 0b01101110,
    //  0b01100110, 0b00111100},
    // {0b00000000, 0b01100110, 0b01100110, 0b01100110, 0b01111110, 0b01100110,
    //  0b01100110, 0b01100110},
    // {0b00000000, 0b00111100, 0b00011000, 0b00011000, 0b00011000, 0b00011000,
    //  0b00011000, 0b00111100},
    // {0b00000000, 0b00011110, 0b00001100, 0b00001100, 0b00001100, 0b01101100,
    //  0b01101100, 0b00111000},
    // {0b00000000, 0b01100110, 0b01101100, 0b01111000, 0b01110000, 0b01111000,
    //  0b01101100, 0b01100110},
    // {0b00000000, 0b01100000, 0b01100000, 0b01100000, 0b01100000, 0b01100000,
    //  0b01100000, 0b01111110},
    // {0b00000000, 0b01100011, 0b01110111, 0b01111111, 0b01101011, 0b01100011,
    //  0b01100011, 0b01100011},
    // {0b00000000, 0b01100011, 0b01110011, 0b01111011, 0b01101111, 0b01100111,
    //  0b01100011, 0b01100011},
    // {0b00000000, 0b00111100, 0b01100110, 0b01100110, 0b01100110, 0b01100110,
    //  0b01100110, 0b00111100},
    // {0b00000000, 0b01111100, 0b01100110, 0b01100110, 0b01100110, 0b01111100,
    //  0b01100000, 0b01100000},
    // {0b00000000, 0b00111100, 0b01100110, 0b01100110, 0b01100110, 0b01101110,
    //  0b00111100, 0b00000110},
    // {0b00000000, 0b01111100, 0b01100110, 0b01100110, 0b01111100, 0b01111000,
    //  0b01101100, 0b01100110},
    // {0b00000000, 0b00111100, 0b01100110, 0b01100000, 0b00111100, 0b00000110,
    //  0b01100110, 0b00111100},
    // {0b00000000, 0b01111110, 0b01011010, 0b00011000, 0b00011000, 0b00011000,
    //  0b00011000, 0b00011000},
    // {0b00000000, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b01100110,
    //  0b01100110, 0b00111110},
    // {0b00000000, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b01100110,
    //  0b00111100, 0b00011000},
    // {0b00000000, 0b01100011, 0b01100011, 0b01100011, 0b01101011, 0b01111111,
    //  0b01110111, 0b01100011},
    // {0b00000000, 0b01100011, 0b01100011, 0b00110110, 0b00011100, 0b00110110,
    //  0b01100011, 0b01100011},
    // {0b00000000, 0b01100110, 0b01100110, 0b01100110, 0b00111100, 0b00011000,
    //  0b00011000, 0b00011000},
    // {0b00000000, 0b01111110, 0b00000110, 0b00001100, 0b00011000, 0b00110000,
    //  0b01100000, 0b01111110},
    // {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    //  0b00000000, 0b00000000},
    // {0b00000000, 0b00000000, 0b00000000, 0b00111100, 0b00000110, 0b00111110,
    //  0b01100110, 0b00111110},
    // {0b00000000, 0b01100000, 0b01100000, 0b01100000, 0b01111100, 0b01100110,
    //  0b01100110, 0b01111100},
    // {0b00000000, 0b00000000, 0b00000000, 0b00111100, 0b01100110, 0b01100000,
    //  0b01100110, 0b00111100},
    // {0b00000000, 0b00000110, 0b00000110, 0b00000110, 0b00111110, 0b01100110,
    //  0b01100110, 0b00111110},
    // {0b00000000, 0b00000000, 0b00000000, 0b00111100, 0b01100110, 0b01111110,
    //  0b01100000, 0b00111100},
    // {0b00000000, 0b00011100, 0b00110110, 0b00110000, 0b00110000, 0b01111100,
    //  0b00110000, 0b00110000},
    // {0b00000000, 0b00000000, 0b00111110, 0b01100110, 0b01100110, 0b00111110,
    //  0b00000110, 0b00111100},
    // {0b00000000, 0b01100000, 0b01100000, 0b01100000, 0b01111100, 0b01100110,
    //  0b01100110, 0b01100110},
    // {0b00000000, 0b00000000, 0b00011000, 0b00000000, 0b00011000, 0b00011000,
    //  0b00011000, 0b00111100},
    // {0b00000000, 0b00001100, 0b00000000, 0b00001100, 0b00001100, 0b01101100,
    //  0b01101100, 0b00111000},
    // {0b00000000, 0b01100000, 0b01100000, 0b01100110, 0b01101100, 0b01111000,
    //  0b01101100, 0b01100110},
    // {0b00000000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000,
    //  0b00011000, 0b00011000},
    // {0b00000000, 0b00000000, 0b00000000, 0b01100011, 0b01110111, 0b01111111,
    //  0b01101011, 0b01101011},
    // {0b00000000, 0b00000000, 0b00000000, 0b01111100, 0b01111110, 0b01100110,
    //  0b01100110, 0b01100110},
    // {0b00000000, 0b00000000, 0b00000000, 0b00111100, 0b01100110, 0b01100110,
    //  0b01100110, 0b00111100},
    // {0b00000000, 0b00000000, 0b01111100, 0b01100110, 0b01100110, 0b01111100,
    //  0b01100000, 0b01100000},
    // {0b00000000, 0b00000000, 0b00111100, 0b01101100, 0b01101100, 0b00111100,
    //  0b00001101, 0b00001111},
    // {0b00000000, 0b00000000, 0b00000000, 0b01111100, 0b01100110, 0b01100110,
    //  0b01100000, 0b01100000},
    // {0b00000000, 0b00000000, 0b00000000, 0b00111110, 0b01000000, 0b00111100,
    //  0b00000010, 0b01111100},
    // {0b00000000, 0b00000000, 0b00011000, 0b00011000, 0b01111110, 0b00011000,
    //  0b00011000, 0b00011000},
    // {0b00000000, 0b00000000, 0b00000000, 0b01100110, 0b01100110, 0b01100110,
    //  0b01100110, 0b00111110},
    // {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b01100110, 0b01100110,
    //  0b00111100, 0b00011000},
    // {0b00000000, 0b00000000, 0b00000000, 0b01100011, 0b01101011, 0b01101011,
    //  0b01101011, 0b00111110},
    // {0b00000000, 0b00000000, 0b00000000, 0b01100110, 0b00111100, 0b00011000,
    //  0b00111100, 0b01100110},
    // {0b00000000, 0b00000000, 0b00000000, 0b01100110, 0b01100110, 0b00111110,
    //  0b00000110, 0b00111100},
    // {0b00000000, 0b00000000, 0b00000000, 0b00111100, 0b00001100, 0b00011000,
    //  0b00110000, 0b00111100},
};

const unsigned char IMAGES_UB_LEN = sizeof(IMAGES_UP_BOTTOM) / 8;

const unsigned char IMAGES_LEFT_RIGHT[][8] = {
    {0b11111111, 0b00000000, 0b00000000, 0b00000011, 0b00000011, 0b00000000,
     0b00000000, 0b11111111},
    {0b11111111, 0b00000000, 0b00000000, 0b00000110, 0b00000110, 0b00000000,
     0b00000000, 0b11111111},
    {0b11111111, 0b00000000, 0b00000000, 0b00001100, 0b00001100, 0b00000000,
     0b00000000, 0b11111111},
    {0b11111111, 0b00000000, 0b00000000, 0b00011000, 0b00011000, 0b00000000,
     0b00000000, 0b11111111},
    {0b11111111, 0b00000000, 0b00000000, 0b00110000, 0b00110000, 0b00000000,
     0b00000000, 0b11111111},
    {0b11111111, 0b00000000, 0b00000000, 0b01100000, 0b01100000, 0b00000000,
     0b00000000, 0b11111111},
    {0b11111111, 0b00000000, 0b00000000, 0b11000000, 0b11000000, 0b00000000,
     0b00000000, 0b11111111},

    {0b11111111, 0b00000000, 0b00000000, 0b10000000, 0b10000000, 0b00000000,
     0b00000000, 0b11111111},

    {0b11111111, 0b00000000, 0b00000000, 0b11000000, 0b11000000, 0b00000000,
     0b00000000, 0b11111111},
    {0b11111111, 0b00000000, 0b00000000, 0b01100000, 0b01100000, 0b00000000,
     0b00000000, 0b11111111},
    {0b11111111, 0b00000000, 0b00000000, 0b00110000, 0b00110000, 0b00000000,
     0b00000000, 0b11111111},
    {0b11111111, 0b00000000, 0b00000000, 0b00011000, 0b00011000, 0b00000000,
     0b00000000, 0b11111111},
    {0b11111111, 0b00000000, 0b00000000, 0b00001100, 0b00001100, 0b00000000,
     0b00000000, 0b11111111},
    {0b11111111, 0b00000000, 0b00000000, 0b00000011, 0b00000011, 0b00000000,
     0b00000000, 0b11111111},
    {0b11111111, 0b00000000, 0b00000000, 0b00000001, 0b00000001, 0b00000000,
     0b00000000, 0b11111111},
};

const unsigned char IMAGES_LR_LEN = sizeof(IMAGES_LEFT_RIGHT) / 8;

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
// void InterruptTime1() __interrupt(3) {
// // setup TH0 TL0 initial value, each interrupt(Timer0 overflow) will pass
// 1ms TH1 = 0xFC; TL1 = 0x67;
//
// INTERRUPT_COUNT++;
// if (INTERRUPT_COUNT >= INTERRUPT_MILLIS) {  // 1ms * INTERRUPT_MILLIS
//   INTERRUPT_COUNT = 0;
//   INTERRUPT_FLAG = 1;
// }
//
// enable_led_row(digit_row());
// enable_led_column(digit_col());
// }

unsigned char bit_revert(unsigned char a) {
  unsigned char r = 0;
  for (unsigned char i = 0; i < 8; ++i) {
    r |= ((a & (1 << i)) >> i) << (7 - i);
  }
  return r;
}

void show_heart_image() {
  turn_off_led_matrix();
  switch (HEART_IMAGE_IDX) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
      enable_led_row(HEART_IMAGE_IDX);
      P0 = 0xFF ^ bit_revert(HEART_IMAGE[HEART_IMAGE_IDX]);
      HEART_IMAGE_IDX++;
      break;
    case 7:
      enable_led_row(HEART_IMAGE_IDX);
      P0 = 0xFF ^ bit_revert(HEART_IMAGE[HEART_IMAGE_IDX]);
      HEART_IMAGE_IDX = 0;
      break;
    default:
      break;
  }
}

void show_digit_image() {
  turn_off_led_matrix();
  switch (DIGIT_IMAGE_IDX) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
      enable_led_row(DIGIT_IMAGE_IDX);
      P0 = 0xff ^
           bit_revert(IMAGES_UP_BOTTOM[SHOW_IMAGE_ROUND][DIGIT_IMAGE_IDX]);
      DIGIT_IMAGE_IDX++;
      break;
    case 7:
      enable_led_row(DIGIT_IMAGE_IDX);
      P0 = 0xff ^
           bit_revert(IMAGES_UP_BOTTOM[SHOW_IMAGE_ROUND][DIGIT_IMAGE_IDX]);
      DIGIT_IMAGE_IDX = 0;
      break;
    default:
      break;
  }
}

void show_digit_image_dynamic_up_to_bottom() {
  turn_off_led_matrix();
  unsigned char row = 0, col = 0;
  switch (DIGIT_IMAGE_IDX) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
      // 动态显示的时候把整个二位数组打平为一维数组，则动态显示的问题就变成：
      // 当前走了 n 个周期一共走过了多少个行，当前定位在二位数组的哪个 row 哪个
      // col，在确定 row 时在走过的周期上再加上对应的 LED 行数
      enable_led_row(DIGIT_IMAGE_IDX);
      row = (SHOW_IMAGE_ROUND + DIGIT_IMAGE_IDX) / 8;
      col = SHOW_IMAGE_ROUND + DIGIT_IMAGE_IDX - row * 8;
      // 使用 row % IMAGES_UB_LEN 消除循环完一个周期后的衔接问题
      P0 = 0xff ^ bit_revert(IMAGES_UP_BOTTOM[row % IMAGES_UB_LEN][col]);
      DIGIT_IMAGE_IDX++;
      break;
    case 7:
      enable_led_row(DIGIT_IMAGE_IDX);
      row = (SHOW_IMAGE_ROUND + DIGIT_IMAGE_IDX) / 8;
      col = SHOW_IMAGE_ROUND + DIGIT_IMAGE_IDX - row * 8;
      P0 = 0xff ^ bit_revert(IMAGES_UP_BOTTOM[row % IMAGES_UB_LEN][col]);
      DIGIT_IMAGE_IDX = 0;
      break;
    default:
      break;
  }
}

void show_digit_image_dynamic_left_to_right() {
  turn_off_led_matrix();
  unsigned char row = 0, col = 0;
  switch (DIGIT_IMAGE_IDX) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
      enable_led_row(DIGIT_IMAGE_IDX);
      P0 = 0xff ^
           bit_revert(IMAGES_LEFT_RIGHT[SHOW_IMAGE_ROUND][DIGIT_IMAGE_IDX]);
      DIGIT_IMAGE_IDX++;
      break;
    case 7:
      enable_led_row(DIGIT_IMAGE_IDX);
      P0 = 0xff ^
           bit_revert(IMAGES_LEFT_RIGHT[SHOW_IMAGE_ROUND][DIGIT_IMAGE_IDX]);
      DIGIT_IMAGE_IDX = 0;
      break;
    default:
      break;
  }
}

_Noreturn void show_image_on_led_matrix_up_bottom() {
  enable_u4_74hc138();
  P0 = 0xff;

  while (1) {
    run_in_every_ms(100, &show_digit_image_dynamic_up_to_bottom);
    turn_off_led_matrix();
    delay_ms(10);
    SHOW_IMAGE_ROUND++;
    if (SHOW_IMAGE_ROUND >= IMAGES_LR_LEN * 8) {
      SHOW_IMAGE_ROUND = 0;
    }
  }
}

_Noreturn void show_image_on_led_matrix_left_right() {
  enable_u4_74hc138();
  P0 = 0xff;

  while (1) {
    run_in_every_ms(100, &show_digit_image_dynamic_left_to_right);
    turn_off_led_matrix();
    delay_ms(10);
    SHOW_IMAGE_ROUND++;
    if (SHOW_IMAGE_ROUND >= IMAGES_LR_LEN) {
      SHOW_IMAGE_ROUND = 0;
    }
  }
}

_Noreturn void turn_on_led_matrix() {
  // turn_on_led_matrix_normal();
  // turn_on_led_matrix_with_interrupt(100);
  // show_image_on_led_matrix_up_bottom();
  show_image_on_led_matrix_left_right();
}
