// $Id: mathutil.cc,v 1.6 2001-05-22 18:03:59 jle Exp $

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

u_fp32 distance_to_line_sqr(
  fp16 x, fp16 y, fp16 xs, fp16 ys, fp16 xd, fp16 yd) {

  // Check if actually a point
  if (xd == 0 && yd == 0)
    return vector_length_sqr_fp16_fp32(x - xs, y - ys);

  // Calculate the multiplier
  fp16 projection = project_point_to_vector(x - xs, y - ys, xd, yd);
  
  // If the starting point is the closest...
  if (projection <= 0)
    return vector_length_sqr_fp16_fp32(x - xs, y - ys);

  // ...else if the ending point is the closest...
  else if (projection >= 0x10000)
    return vector_length_sqr_fp16_fp32(x - (xs + xd), y - (ys + yd));

  // ...else the closest point is somewhere along the line
  else {
    fp16 xi = xs + mul_fp16(projection, xd);
    fp16 yi = ys + mul_fp16(projection, yd);
    return vector_length_sqr_fp16_fp32(x - xi, y - yi);
  }
}

fp16 project_point_to_vector(fp16 x, fp16 y, fp16 xv, fp16 yv) {
  fp32 dp = dot_product(x, y, xv, yv);
  u_fp32 len = vector_length_sqr_fp16_fp32(xv, yv);
  return div_fp32_fp32_fp16(dp, len);
}
