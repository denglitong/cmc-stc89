//
// Created by Litong Deng on 2022/10/4.
//

#include "common.h"

void delay_ms(unsigned int s) {
  unsigned int x;
  for (s; s > 0; s--) {
    x = 98;
    while (x--)
      ;
  }
}
