// $Id: mathutil.hpp,v 1.1 2001-05-08 21:20:02 jle Exp $

#ifndef __transform_hpp_INCLUDED__
#define __transform_hpp_INCLUDED__

/**
 * Multiplies two 16.16 fixed point values returning the result as integer.
 *
 * @param a the first value (16.16 fixed)
 * @param b the second value (16.16 fixed)
 * @return the result as integer
 */
inline int mul_fp16_fp16_int(int a, int b) {

#ifdef USE_MIPS_ASM

  int res;
  asm("mult %1, %2\n"
      "mfhi %0\n"
      : "=r" (res)
      : "r" (a), "r" (b)
      : "$9", "$10", "lo", "hi", "cc");
  return res;

#else

  return ((a >> 8) * (b >> 8)) >> 16;

#endif

}

/**
 * Multiplies two 16.16 fixed point values.
 *
 * @param a the first value (16.16 fixed)
 * @param b the second value (16.16 fixed)
 * @return the result (16.16 fixed)
 */
inline int mul_fp16(int a, int b) {

#ifdef USE_MIPS_ASM

  int res;
  asm("mult %1, %2\n"
      "mflo $9\n"
      "mfhi $10\n"
      "srl $9, $9, 16\n"
      "sll $10, $10, 16\n"
      "or %0, $9, $10"
      : "=r" (res)
      : "r" (a), "r" (b)
      : "$9", "$10", "lo", "hi", "cc");
  return res;

#else // USE_MIPS_ASM

  return (a >> 8) * (b >> 8);

#endif // USE_MIPS_ASM

}

/**
 * Divides integer by 16.16 fixed point value.
 *
 * @param dividend the dividend (integer)
 * @param divisor the divisor (16.16 fixed)
 * @return the result (16.16 fixed)
 */
int div_int_fp16u_fp16(int dividend, unsigned int divisor);

/**
 * Divides two 16.16 fixed point values.
 *
 * @param a the first value (16.16 fixed)
 * @param b the second value (16.16 fixed)
 * @return the result (16.16 fixed)
 */
int div_fp16(int dividend, int divisor);

/**
 * Calculates the length of the specified vector.
 *
 * @param x the x component of the vector (16.16 fixed)
 * @param y the y component of the vector (16.16 fixed)
 * @return the square of the length of the vector (16.16 fixed)
 */
inline int vector_length(int x, int y) {
  return mul_fp16(x, x) + mul_fp16(y, y);
}

/**
 * Calculates the intersection point of the two straights.
 *
 * @param xs1 the x coordinate of the starting point 1 (16.16 fixed)
 * @param ys1 the y coordinate of the starting point 1 (16.16 fixed)
 * @param xd1 the x component of the direction 1 (16.16 fixed)
 * @param yd1 the y component of the direction 1 (16.16 fixed)
 * @param xs2 the x coordinate of the starting point 2 (16.16 fixed)
 * @param ys2 the y coordinate of the starting point 2 (16.16 fixed)
 * @param xd2 the x component of the direction 2 (16.16 fixed)
 * @param yd2 the y component of the direction 2 (16.16 fixed)
 * @param xi the x coordinate of the intersection point (16.16 fixed)
 * @param yi the y coordinate of the intersectino point (16.16 fixed)
 */
void intersect_straights(int xs1, int ys1, int xd1, int yd1,
                         int xs2, int ys2, int xd2, int yd2,
                         int &xi, int &yi);

/**
 * Calculates how far the specified point is from the specified
 * line segment.
 *
 * @param x the x coordinate of the point (16.16 fixed)
 * @param y the y coordinate of the point (16.16 fixed)
 * @param xs the x coordinate of the line end point (16.16 fixed)
 * @param ys the y coordinate of the line end point (16.16 fixed)
 * @param xd the x component of the line vector (16.16 fixed)
 * @param yd the y component of the line vector (16.16 fixed)
 * @return the minimum distance squared (16.16 fixed)
 */
int distance_from_line(int x, int y, int xs, int ys, int xd, int yd);

inline int coord_to_screen(int c, int origin, unsigned int scaling) {
  return mul_fp16_fp16_int(c - origin, scaling);
}

inline int screen_to_coord(int sc, int origin, unsigned int scaling) {
  return div_int_fp16u_fp16(sc, scaling) + origin;
}

#endif
