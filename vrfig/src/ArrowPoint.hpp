// $Id: ArrowPoint.hpp,v 1.1 2004-11-10 14:03:05 jle Exp $

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

#ifndef __ArrowPoint_hpp_INCLUDED__
#define __ArrowPoint_hpp_INCLUDED__

#include "mathutil.hpp"

/**
 * An object representing an arrow supporting point. The arrow size is
 * determined by the length of the direction vector (which is just rotated
 * to draw the arrow sides).
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.1 $
 */
struct ArrowPoint : public Point {

  /** The x component of the arrow direction */
  fp16 dir_x;

  /** The y component of the arrow direction */
  fp16 dir_y;

  /** Whether there is an arrow at this point */
  bool has_arrow;

  inline ArrowPoint() : Point(), has_arrow(false) {}

  inline ArrowPoint(fp16 x, fp16 y) : Point(x, y), has_arrow(false) {}

  inline ArrowPoint(fp16 x, fp16 y, fp16 dir_x, fp16 dir_y)
    : Point(x, y), dir_x(dir_x), dir_y(dir_y), has_arrow(true) {}

  /**
   * Draws the arrow if there is an arrow at this point. Assumes that the
   * drawing parameters have already been setup.
   *
   * @param origin_x the x coordinate for the origin 
   * @param origin_y the y coordinate for the origin 
   * @param scaling the scaling used 
   */
  void draw(fp16 origin_x, fp16 origin_y, u_fp16 scaling) const;
};

#endif
