// $Id: transform.cc,v 1.1 2001-05-07 21:02:10 jle Exp $

#include "transform.hpp"

int mul_fp16_fp16u_int(int a, unsigned int b) {
  return ((a >> 8) * (b >> 8)) >> 16;
}

int div_int_fp16u_fp16(int dividend, unsigned int divisor) {
  return ((dividend << 16) / (divisor >> 8)) << 8;
}
