// $Id: transform.hpp,v 1.1 2001-05-07 21:02:11 jle Exp $

#ifndef __transform_hpp_INCLUDED__
#define __transform_hpp_INCLUDED__

int mul_fp16_fp16u_int(int a, unsigned int b);
int div_int_fp16u_fp16(int dividend, unsigned int divisor);

inline int coord_to_screen(int c, int origin, unsigned int scaling) {
  return mul_fp16_fp16u_int(c - origin, scaling);
}

inline int screen_to_coord(int sc, int origin, unsigned int scaling) {
  return div_int_fp16u_fp16(sc, scaling) + origin;
}

#endif
