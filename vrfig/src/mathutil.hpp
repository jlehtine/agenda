// $Id: mathutil.hpp,v 1.12 2001-06-10 19:35:13 jle Exp $

/*--------------------------------------------------------------------------
 * VRFig, a vector graphics editor for PDA environment
 * Copyright 2001 Johannes Lehtinen
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *------------------------------------------------------------------------*/

#ifndef __mathutil_hpp_INCLUDED__
#define __mathutil_hpp_INCLUDED__

#include <iostream.h>

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

inline int fp16_to_int(fp16 i) {
  return i >> 16;
}

inline fp32 int_to_fp32(int i) {
  return static_cast<fp32>(i) << 32;
}

inline int fp32_to_int(fp32 i) {
  return i >> 32;
}

inline fp32 fp16_to_fp32(fp16 i) {
  return static_cast<fp32>(i) << 16;
}

inline fp16 fp32_to_fp16(fp32 i) {
  return i >> 16;
}

inline u_fp16 u_fp32_to_u_fp16(u_fp32 i) {
  return i >> 16;
}

/**
 * Converts 16.16 fixed point number to decimal string.
 *
 * @param v the value to convert to string
 * @param str the target string
 * @param n the maximum length of the string
 * @return the number of characters written to the string (excluding null)
 */
size_t fp16_to_str(fp16 v, char *str, size_t n);

/**
 * Writes 16.16 fixed point number to the specified output stream.
 * The number is written in decimal format.
 *
 * @param os the output stream
 * @param v the fixed point value to write
 */
inline ostream &write_fp16(ostream &os, fp16 v) {
  char str[20];
  fp16_to_str(v, str, 20);
  return os << str;
}

/**
 * Converts a decimal string to 16.16 fixed point number.
 *
 * @param str the string to convert
 * @return the fixed point value
 */
fp16 str_to_fp16(const char *str);

/**
 * Multiplies two 16.16 fixed point values returning the result as integer.
 *
 * @param a the first value 
 * @param b the second value 
 * @return the result as integer
 */
inline int mul_fp16_fp16_int(fp16 a, fp16 b) {
  return ((a >> 8) * (b >> 8)) >> 16;
}

inline int mul_fp32_fp32_int(fp32 a, fp32 b) {
  return ((a >> 16) * (b >> 16)) >> 32;
}

/**
 * Multiplies two 16.16 fixed point values.
 *
 * @param a the first value 
 * @param b the second value 
 * @return the result 
 */
inline fp16 mul_fp16(fp16 a, fp16 b) {
  return (a >> 8) * (b >> 8);
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
inline fp16 div_int_fp16u_fp16(int dividend, u_fp16 divisor) {
  return ((dividend << 16) / (divisor >> 8)) << 8;
}

/**
 * Divides two 16.16 fixed point values.
 *
 * @param a the first value 
 * @param b the second value 
 * @return the result 
 */
inline fp16 div_fp16(fp16 dividend, fp16 divisor) {
  return (static_cast<fp32>(dividend) << 16) / divisor;
}

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

/** Calculates the dot product for the specified vectors.
 *
 * @param x1 the x component of the first vector
 * @param y1 the y component of the first vector
 * @param x2 the x component of the second vector
 * @param y2 the y component of the second vector
 */
inline fp32 dot_product(fp16 x1, fp16 y1, fp16 x2, fp16 y2) {
  return mul_fp16_fp16_fp32(x1, x2) + mul_fp16_fp16_fp32(y1, y2);
}

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
 * @return the minimum distance squared
 */
u_fp32 distance_to_line_sqr(
  fp16 x, fp16 y, fp16 xs, fp16 ys, fp16 xd, fp16 yd);

/**
 * Projects the specified point to the specified origin based vector.
 * Returns the scalar multiplier for the projection point.
 *
 * @param x the x coordinate of the point
 * @param y the y coordinate of the point
 * @param xv the x component of the vector
 * @param xy the y component of the vector
 */
inline fp16 project_point_to_vector(fp16 x, fp16 y, fp16 xv, fp16 yv) {
  fp32 dp = dot_product(x, y, xv, yv);
  u_fp32 len = vector_length_sqr_fp16_fp32(xv, yv);
  return div_fp32_fp32_fp16(dp, len);
}

/**
 * Transforms the specified length from figure coordinates to screen
 * coordinates.
 *
 * @param l the length in figure coordinates
 * @param scaling the scaling factor for the current view
 * @return the length in screen coordinates
 */
inline int length_to_screen(fp16 l, u_fp16 scaling) {
  return mul_fp16_fp16_int(l, scaling);
}

/**
 * Transforms the specified length from screen coordinates to figure
 * coordinates.
 *
 * @param lc the length in screen coordinates
 * @param scaling the scaling factor for the current view
 * @return the length in figure coordinates
 */
inline fp16 length_from_screen(int lc, u_fp16 scaling) {
  return div_int_fp16u_fp16(lc, scaling);
}

#endif
