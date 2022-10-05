//
// Created by Litong Deng on 2022/10/4.
//

#include "common.h"

void delay_ms(unsigned int s) {
  unsigned int x;
  for (s; s > 0; s--) {
    // 这里 x 设置为 98 猜测是和晶振电路的频率有关
    x = 98;
    while (x--)
      ;
  }
}
