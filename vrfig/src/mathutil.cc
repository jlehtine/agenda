// $Id: mathutil.cc,v 1.3 2001-05-19 08:42:17 jle Exp $

#include "mathutil.hpp"

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

int distance_from_line(fp16 x, fp16 y, fp16 xs, fp16 ys, fp16 xd, fp16 yd) {

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
