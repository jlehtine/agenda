// $Id: mathutil.cc,v 1.5 2001-05-20 18:51:23 jle Exp $

#include <stdio.h>
#include <string.h>
#include "mathutil.hpp"

size_t fp16_to_str(fp16 v, char *str, size_t n) {
  size_t num_written = 0;
  if (num_written >= n)
    return num_written;
  if (v < 0) {
    *(str++) = '-';
    num_written++;
    if (num_written >= n)
      return num_written;
    v = -v;
  }
  num_written += snprintf(str, n - num_written, "%u", (unsigned int)(v >> 16));
  if (num_written >= n)
    return num_written;
  
  // Add the decimal part to the string if necessary
  v &= 0xffff;
  if (v) {
    char *t = str + strlen(str);
    *(t++) = '.';
    num_written++;
    if (num_written >= n)
      return num_written;
    int num_decimals = 0;
    while (v && num_decimals++ < 6) {
      v *= 10;
      *(t++) = '0' + (v >> 16);
      num_written++;
      if (num_written >= n)
        return num_written;
      v &= 0xffff;
    }
    *t = '\0';
  }
  return num_written;
}

ostream &write_fp16(ostream &os, fp16 v) {
  char str[20];
  fp16_to_str(v, str, 20);
  return os << str;
}

fp16 str_to_fp16(const char *str) {
  fp16 v = 0;
  bool minus_sign;
  if (*str == '-') {
    minus_sign = true;
    str++;
  } else
    minus_sign = false;
  while (*str >= '0' && *str <= '9') {
    v *= 10;
    v += *(str++) - '0';
  }
  v <<= 16;
  if (*str == '\0')
    return minus_sign ? -v : v;
  if (*str != '.')
    return 0;
  str++;
  u_fp16 dp = 0;
  unsigned int divider = 1;
  while (*str >= '0' && *str <= '9' && divider < 1000000000) {
    dp *= 10;
    dp += *(str++) - '0';
    divider *= 10;
  }
  if (divider < 1000000000 && *str != '\0')
    return 0;
  v |= (u_fp16)((((unsigned long long)dp << 16) + (divider >> 1)) / divider);
  return minus_sign ? -v : v;
}

fp16 div_int_fp16u_fp16(int dividend, u_fp16 divisor) {
  return ((dividend << 16) / (divisor >> 8)) << 8;
}

fp16 div_fp16(fp16 dividend, fp16 divisor) {
  u_fp16 mask = 0xffff8000;
  int dividend_shift = 16;
  while ((dividend & mask) != 0 && (dividend & mask) != mask) {
    mask <<= 1;
    dividend_shift--;
  }
  return (dividend << dividend_shift) / (divisor >> (16 - dividend_shift));
}

u_fp16 sqrt_fp32_fp16(u_fp32 square) {
  unsigned int i = 16;
  unsigned int sum = 0;
  unsigned int root = 0;
  unsigned int temp;

  square >>= 16;
  while (i--) {
    square <<= 2;
    sum <<= 1;
    root <<= 1;
    temp = sum + 1;
    if (square >> 32 >= temp) {
      square -= (static_cast<unsigned long long>(temp)) << 32;
      root++;
      sum += 2;
    }
  }
  return root << 8;
}

void intersect_straights(fp16 xs1, fp16 ys1, fp16 xd1, fp16 yd1,
                         fp16 xs2, fp16 ys2, fp16 xd2, fp16 yd2,
                         fp16 &xi, fp16 &yi) {

  //      xd1 * (xd2 * ys2 + xs2 * yd2) - xd2 * (xd1 * ys1 + xs1 * yd1)
  // xi = -------------------------------------------------------------
  //                        xd2 * yd1 + xd1 * yd2
  xi = div_fp32_fp32_fp16(
    mul_fp16_fp16_fp32(xd1, mul_fp16(xd2, ys2) + mul_fp16(xs2, yd2))
    - mul_fp16_fp16_fp32(xd2, mul_fp16(xd1, ys1) + mul_fp16(xs1, yd1)),
    mul_fp16_fp16_fp32(xd2, yd1) - mul_fp16_fp16_fp32(xd1, yd2));
  
  //      yd1 * (yd2 * xs2 + ys2 * xd2) - yd2 * (yd1 * xs1 + ys1 * xd1)
  // yi = -------------------------------------------------------------
  //                        yd2 * xd1 + yd1 * xd2
  yi = div_fp32_fp32_fp16(
    mul_fp16_fp16_fp32(yd1, mul_fp16(yd2, xs2) + mul_fp16(ys2, xd2))
    - mul_fp16_fp16_fp32(yd2, mul_fp16(yd1, xs1) + mul_fp16(ys1, xd1)),
    mul_fp16_fp16_fp32(yd2, xd1) - mul_fp16_fp16_fp32(yd1, xd2));
}

u_fp16 distance_to_line(fp16 x, fp16 y, fp16 xs, fp16 ys, fp16 xd, fp16 yd) {

  // Check if actually a point
  if (xd == 0 && yd == 0)
    return vector_length_fp16(x - xs, y - ys);

  // Direction for a line making 90 degree angle with the specified line
  fp16 xdc = -yd;
  fp16 ydc = xd;
  
  // Calculate the intersection point
  fp16 xi, yi;
  intersect_straights(x, y, xdc, ydc, xs, ys, xd, yd, xi, yi);
  
  // Check if the intersection point is on the line segment
  if (mul_fp16_fp16_fp32(xi - xs, xd) + mul_fp16_fp16_fp32(yi - ys, yd) < 0) {

    // Closest to start point
    return vector_length_fp16(x - xs, y - ys);

  } else if (vector_length_fp16(xi - xs, yi - ys) > 
             vector_length_fp16(xd, yd)) {
    
    // Closest to end point
    return vector_length_fp16(x - (x + xd), y - (y + yd));

  } else {
    
    // Closest to intersection point
    return vector_length_fp16(x - xi, y - yi);

  }
}
