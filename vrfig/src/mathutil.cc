// $Id: mathutil.cc,v 1.2 2001-05-16 19:50:16 jle Exp $

#include "mathutil.hpp"

int div_int_fp16u_fp16(int dividend, unsigned int divisor) {
  return ((dividend << 16) / (divisor >> 8)) << 8;
}

int div_fp16(int dividend, int divisor) {
  unsigned int mask = 0xffff8000;
  int dividend_shift = 16;
  while ((dividend & mask) != 0 && (dividend & mask) != mask) {
    mask <<= 1;
    dividend_shift--;
  }
  return (dividend << dividend_shift) / (divisor >> (16 - dividend_shift));
}

void intersect_straights(int xs1, int ys1, int xd1, int yd1,
                         int xs2, int ys2, int xd2, int yd2,
                         int &xi, int &yi) {

  //      xd1 * (xd2 * ys2 + xs2 * yd2) - xd2 * (xd1 * ys1 + xs1 * yd1)
  // xi = -------------------------------------------------------------
  //                        xd2 * yd1 + xd1 * yd2
  xi = div_fp16(mul_fp16(xd1,
                         mul_fp16(xd2, ys2) + mul_fp16(xs2, yd2))
                - mul_fp16(xd2,
                           mul_fp16(xd1, ys1) + mul_fp16(xs1, yd1)),
                mul_fp16(xd2, yd1) - mul_fp16(xd1, yd2));

  //      yd1 * (yd2 * xs2 + ys2 * xd2) - yd2 * (yd1 * xs1 + ys1 * xd1)
  // yi = -------------------------------------------------------------
  //                        yd2 * xd1 + yd1 * xd2
  yi = div_fp16(mul_fp16(yd1,
                         mul_fp16(yd2, xs2) + mul_fp16(ys2, xd2))
                - mul_fp16(yd2,
                           mul_fp16(yd1, xs1) + mul_fp16(ys1, xd1)),
                mul_fp16(yd2, xd1) - mul_fp16(yd1, xd2));
}

int distance_from_line(int x, int y, int xs, int ys, int xd, int yd) {

  // Check if actually a point
  if (xd == 0 && yd == 0)
    return vector_length(x - xs, y - ys);

  // Direction for a line making 90 degree angle with the specified line
  int xdc = -yd;
  int ydc = xd;
  
  // Calculate the intersection point
  int xi, yi;
  intersect_straights(x, y, xdc, ydc, xs, ys, xd, yd, xi, yi);
  
  // Check if the intersection point is on the line segment
  if (mul_fp16(xi - xs, xd) + mul_fp16(yi - ys, yd) < 0) {

    // Closest to start point
    return vector_length(x - xs, y - ys);

  } else if (vector_length(xi - xs, yi - ys) > vector_length(xd, yd)) {
    
    // Closest to end point
    return vector_length(x - (x + xd), y - (y + yd));

  } else {
    
    // Closest to intersection point
    return vector_length(x - xi, y - yi);

  }
}
