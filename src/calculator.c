//
// Created by Litong Deng on 2022/10/14.
//

#include <8051.h>

#define ADDR_0 P1_0
#define ADDR_1 P1_1
#define ADDR_2 P1_2
#define ADDR_3 P1_3
#define EN_LED P1_4

#define LED_LINE P0
#define DOT 10

#define ADD 'A'
#define MINUS 'B'
#define MULTIPLE 'C'
#define DIVIDE 'D'
#define EQUAL 'E'
#define CLEAR 'F'

typedef void(VoidFun)(unsigned long);

unsigned char MATRIX_KEY_STATUS[4][4] = {
    {1, 1, 1, 1},
    {1, 1, 1, 1},
    {1, 1, 1, 1},
    {1, 1, 1, 1},
};

unsigned char LED_CHAR[] = {
    0xff ^ 0b111111,  0xff ^ 0b110,     0xff ^ 0b1011011, 0xff ^ 0b1001111,
    0xff ^ 0b1100110, 0xff ^ 0b1101101, 0xff ^ 0b1111101, 0xff ^ 0b111,
    0xff ^ 0b1111111, 0xff ^ 0b1101111,
};

unsigned char LED_BUFF[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};

void enable_u3_74hc138() {
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

/**
 * map 4 * 4 input keys to [0-9A-F]
 * keyboard layout:
 *  # # #     #
 *  # # #   #   #
 *  # # #     #
 *  #       #   #
 * @param key_row
 * @param key_col
 * @return
 */
unsigned char input_digit(unsigned char key_row, unsigned char key_col) {
  static unsigned char digit_buffer[4][4] = {
      {1, 2, 3, ADD},
      {4, 5, 6, MULTIPLE},
      {7, 8, 9, MINUS},
      {0, EQUAL, CLEAR, DIVIDE},
  };
  return digit_buffer[key_row][key_col];
}

void update_led_buffer(unsigned long digit) {
  signed char i = 0;
  unsigned char buf[6];
  do {
    buf[i++] = digit % 10;
    digit /= 10;
  } while (digit > 0);

  for (i = 5; i >= 1; i--) {
    if (buf[i] == 0) {
      buf[i] = 0xFF;
    } else {
      break;
    }
  }
  for (; i >= 0; i--) {
    LED_BUFF[i] = LED_CHAR[buf[i]];
  }
}

void react_to_input_digit(unsigned char input_key) {
  // 结果
  static unsigned long RESULT = 0;
  // + - * / 操作数
  static unsigned long OPERAND = 0;
  // 操作符缓存
  static unsigned char OPERATOR_BUFF = 0;
  switch (input_key) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
      OPERAND = OPERAND * 10 + input_key;
      update_led_buffer(OPERAND);
      break;
    case ADD:
      RESULT += OPERAND;
      OPERAND = 0;
      update_led_buffer(RESULT);
      break;
    case EQUAL:
      RESULT += OPERAND;
      OPERAND = 0;
      update_led_buffer(RESULT);
      break;
    case CLEAR:
      RESULT = 0;
      OPERAND = 0;
      update_led_buffer(0);
      break;
    default:
      break;
  }
}

_Noreturn void get_matrix_input_key_with_interrupt() {
  enable_u3_74hc138();

  EA = 1;   // enable global interrupt
  ET0 = 1;  // enable Timer0 interrupt

  // setup T0_M1 = 0, T0_M0 = 1 (Timer0 mode TH0-TL0 16 bits timer)
  TMOD = 0x01;
  // setup TH0 TL0 initial value
  TH0 = 0xFC;
  TL0 = 0x67;
  TR0 = 1;  // start/enable Timer0

  LED_BUFF[0] = LED_CHAR[0];

  unsigned char key_out = 0, key_in = 0, digit = 0;
  static unsigned char MATRIX_PREV_KEY_STATUS[4][4] = {
      {1, 1, 1, 1},
      {1, 1, 1, 1},
      {1, 1, 1, 1},
      {1, 1, 1, 1},
  };

  while (1) {
    for (key_out = 0; key_out < 4; ++key_out) {
      for (key_in = 0; key_in < 4; ++key_in) {
        if (MATRIX_PREV_KEY_STATUS[key_out][key_in] !=
            MATRIX_KEY_STATUS[key_out][key_in]) {
          // 这里结合 key_out, key_in 就可以知道是按下了哪个按键
          if (MATRIX_KEY_STATUS[key_out][key_in] == 0) {
            digit = input_digit(key_out, key_in);
            react_to_input_digit(digit);
          }
          MATRIX_PREV_KEY_STATUS[key_out][key_in] =
              MATRIX_KEY_STATUS[key_out][key_in];
        }
      }
    }
  }
}

void scan_keyboard() {
  static unsigned short MATRIX_KEY_BUFFER[4][4] = {
      {0xFF, 0xFF, 0xFF, 0xFF},
      {0xFF, 0xFF, 0xFF, 0xFF},
      {0xFF, 0xFF, 0xFF, 0xFF},
      {0xFF, 0xFF, 0xFF, 0xFF},
  };

  // matrix key input
  unsigned char key_out = 0, key_in = 0, key_in_i;
  for (key_out = 0; key_out < 4; ++key_out) {
    // enable KeyOut_i
    P2 &= (0xF0 | ~((1 << (3 - key_out))));
    for (key_in = 0; key_in < 4; ++key_in) {
      // get KeyIn_i
      key_in_i = (P2 >> (4 + key_in)) & 1;
      MATRIX_KEY_BUFFER[key_out][key_in] =
          (MATRIX_KEY_BUFFER[key_out][key_in] << 1) | key_in_i;
      if (MATRIX_KEY_BUFFER[key_out][key_in] == 0x0000) {
        MATRIX_KEY_STATUS[key_out][key_in] = 0;
      } else if (MATRIX_KEY_BUFFER[key_out][key_in] == 0xFFFF) {
        MATRIX_KEY_STATUS[key_out][key_in] = 1;
      }
    }
  }
}

void flush_led_buffer() {
  static unsigned char TUBE_IDX = 0;
  switch (TUBE_IDX) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
      enable_tube(TUBE_IDX);
      P0 = LED_BUFF[TUBE_IDX];
      TUBE_IDX++;
      break;
    case 5:
      enable_tube(TUBE_IDX);
      P0 = LED_BUFF[TUBE_IDX];
      TUBE_IDX = 0;
      break;
  }
}

void InterruptTime0_key() __interrupt(1) {
  // setup TH0 TL0 initial value, each interrupt(Timer0 overflow) will pass 1ms
  TH0 = 0xFC;
  TL0 = 0x67;

  // 再次的，状态的检测/展示和控制的更新/控制 分离
  // 1.在中断里面检测输入，但是在 main() 里面响应输入的事件
  scan_keyboard();
  // 2.在响应事件里面更新 LED_BUFF，但是刷新确是在每个中断事件里面
  flush_led_buffer();
}

int main() {
  get_matrix_input_key_with_interrupt();
  return 0;
}