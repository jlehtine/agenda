// $Id: ArrowPoint.cc,v 1.1 2004-11-10 14:03:05 jle Exp $

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

#include <FL/fl_draw.H>
#include <FL/Enumerations.H>
#include "ArrowPoint.hpp"
#include "flext.hpp"

// in 16.16 fixed point values
#define SIN_135D (46341)
#define SIN2COS(x) (-(x))

void ArrowPoint::draw(fp16 origin_x, fp16 origin_y, u_fp16 scaling) const {
  if (has_arrow) {

    // Rotate the direction vector as necessary (use precalculated rotation
    // matrices for +/-135 degree rotation)
    fp16 sx = mul_fp16(SIN_135D, dir_x);
    fp16 sy = mul_fp16(SIN_135D, dir_y);
    Point as1(SIN2COS(sx) - sy, sx + SIN2COS(sy));
    Point as2(SIN2COS(sx) + sy, -sx + SIN2COS(sy));

    // Transform the arrow coordinates to screen coordinates
    int as1x, as1y, as2x, as2y, ahx, ahy;
    as1.to_screen(origin_x, origin_y, scaling, as1x, as1y);
    as2.to_screen(origin_x, origin_y, scaling, as2x, as2y);
    to_screen(origin_x, origin_y, scaling, ahx, ahy);

    // Draw the arrow head
    fl_line(as1x, as1y, ahx, ahy, as2x, as2y);
  }
}
