// $Id: mathutil.hpp,v 1.3 2001-05-19 16:55:22 jle Exp $

#ifndef __mathutil_hpp_INCLUDED__
#define __mathutil_hpp_INCLUDED__

/** A signed 16.16 fixed point value */
typedef int fp16;

/** A signed 32.32 fixed point value */
typedef long long fp32;

/** An unsigned 16.16 fixed point value */
typedef unsigned int u_fp16;

/** An unsigned 32.32 fixed point value */
typedef unsigned long long u_fp32;

inline fp16 int_to_fp16(int i) {
  return i << 16;
}

inline fp32 int_to_fp32(int i) {
  return static_cast<fp32>(i) << 32;
}

inline fp32 fp16_to_fp32(fp16 i) {
  return static_cast<fp32>(i) << 16;
}

inline fp16 fp32_to_fp16(fp32 i) {
  return i >> 16;
}

/**
 * Multiplies two 16.16 fixed point values returning the result as integer.
 *
 * @param a the first value 
 * @param b the second value 
 * @return the result as integer
 */
inline int mul_fp16_fp16_int(fp16 a, fp16 b) {

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
 * @param a the first value 
 * @param b the second value 
 * @return the result 
 */
inline fp16 mul_fp16(fp16 a, fp16 b) {

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
 * Multiplies tp 16.16 fixed point value and returns the result as
 * 32.32 fixed point value.
 *
 * @param a the first value 
 * @param b the second value 
 * @return the result
 */
inline fp32 mul_fp16_fp16_fp32(fp16 a, fp16 b) {
  return static_cast<fp32>(a) * static_cast<fp32>(b);
}

/**
 * Divides integer by 16.16 fixed point value.
 *
 * @param dividend the dividend (integer)
 * @param divisor the divisor 
 * @return the result 
 */
fp16 div_int_fp16u_fp16(int dividend, u_fp16 divisor);

/**
 * Divides two 16.16 fixed point values.
 *
 * @param a the first value 
 * @param b the second value 
 * @return the result 
 */
fp16 div_fp16(fp16 dividend, fp16 divisor);

/**
 * Divides two 32.32 fixed point values.
 *
 * @param a the first value
 * @param v the second value
 * @return the result
 */
inline fp16 div_fp32_fp32_fp16(fp32 dividend, fp32 divisor) {
  return dividend / (divisor >> 16);
}

/**
 * Calculates square root of a 32.32 fixed point value.
 *
 * @param s the squared value
 * @return the square root
 */
u_fp16 sqrt_fp32_fp16(u_fp32 s);

/**
 * Calculates the squared length of the specified vector.
 *
 * @param x the x component of the vector 
 * @param y the y component of the vector 
 * @return the squared length of the vector 
 */
inline u_fp32 vector_length_sqr_fp16_fp32(fp16 x, fp16 y) {
  return mul_fp16_fp16_fp32(x, x) + mul_fp16_fp16_fp32(y, y);
}

/**
 * Calculates the length of the specified vector.
 *
 * @param x the x component of the vector 
 * @param y the y component of the vector 
 * @return the length of the vector 
 */
inline u_fp16 vector_length_fp16(fp16 x, fp16 y) {
  return sqrt_fp32_fp16(vector_length_sqr_fp16_fp32(x, y));
}

/**
 * Calculates the squared length of the specified integer vector.
 *
 * @param x the x component of the vector
 * @param y the y component of the vector
 * @return the squared length of the vector
 */
inline unsigned int vector_length_sqr_int(int x, int y) {
  return x*x + y*y;
}

/**
 * Calculates the intersection point of the two straights.
 *
 * @param xs1 the x coordinate of the starting point 1 
 * @param ys1 the y coordinate of the starting point 1 
 * @param xd1 the x component of the direction 1 
 * @param yd1 the y component of the direction 1 
 * @param xs2 the x coordinate of the starting point 2 
 * @param ys2 the y coordinate of the starting point 2 
 * @param xd2 the x component of the direction 2 
 * @param yd2 the y component of the direction 2 
 * @param xi the x coordinate of the intersection point 
 * @param yi the y coordinate of the intersectino point 
 */
void intersect_straights(fp16 xs1, fp16 ys1, fp16 xd1, fp16 yd1,
                         fp16 xs2, fp16 ys2, fp16 xd2, fp16 yd2,
                         fp16 &xi, fp16 &yi);

/**
 * Calculates how close the specified point is to the specified
 * line segment.
 *
 * @param x the x coordinate of the point 
 * @param y the y coordinate of the point 
 * @param xs the x coordinate of the line end point 
 * @param ys the y coordinate of the line end point 
 * @param xd the x component of the line vector 
 * @param yd the y component of the line vector 
 * @return the minimum distance
 */
u_fp16 distance_to_line(fp16 x, fp16 y, fp16 xs, fp16 ys, fp16 xd, fp16 yd);

inline int coord_to_screen(fp16 c, fp16 origin, u_fp16 scaling) {
  return mul_fp16_fp16_int(c - origin, scaling);
}

inline fp16 screen_to_coord(int sc, fp16 origin, u_fp16 scaling) {
  return div_int_fp16u_fp16(sc, scaling) + origin;
}

#endif
