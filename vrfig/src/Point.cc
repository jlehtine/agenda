// $Id: Point.cc,v 1.1 2001-06-10 18:36:43 jle Exp $

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

#include "Point.hpp"

void Point::to_screen(fp16 origin_x, fp16 origin_y, fp16 scaling,
                      int &sx, int &sy) const {
  sx = mul_fp16_fp16_int(x - origin_x, scaling);
  sy = -mul_fp16_fp16_int(y - origin_y, scaling);
}

void Point::from_screen(int sx, int sy, fp16 origin_x, fp16 origin_y, 
                        fp16 scaling) {
  x = div_int_fp16u_fp16(sx, scaling) + origin_x;
  y = -div_int_fp16u_fp16(sy, scaling) + origin_y;
}
