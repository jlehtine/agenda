// $Id: Point.hpp,v 1.2 2001-06-10 19:26:56 jle Exp $

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

#ifndef __Point_hpp_INCLUDED__
#define __Point_hpp_INCLUDED__

#include "mathutil.hpp"
#include "FigureView.hpp"

/** 
 * Type for point coordinate info. The coordinate system used is the
 * usual two dimensional mathematical coordinate system: x axis points
 * to right and y axis points up. Notice, however, that this is _not_
 * the coordinate system used for screen coordinates.
 */
class Point {

public:

  /** The x coordinate */
  fp16 x;

  /** The y coordinate */
  fp16 y;

  Point() {}

  Point(fp16 x, fp16 y): x(x), y(y) {}

  inline bool operator<(Point &p) {
    return (y < p.y || (y == p.y && x < p.x));
  }

  inline bool operator==(Point &p) {
    return (y == p.y && x == p.y);
  }

  /**
   * Transforms this point to screen coordinates using the specified view.
   *
   * @param view the current view
   * @param x the return value for x screen coordinate
   * @param y the return value for y screen coordinate
   */
  inline void to_screen(FigureView *view, int &sx, int &sy) const {
    to_screen(view->get_origin_x(), view->get_origin_y(), view->get_scaling(),
              sx, sy);
  }

  /**
   * Transforms this point to screen coordinates using the specified view
   * parameters.
   *
   * @param origin_x the origin of x screen coordinate axis
   * @param origin_y the origin of y screen coordinate axis
   * @param scaling the scaling for the screen coordinate axis
   * @param x the return value for x screen coordinate
   * @param y the return value for y screen coordinate
   */
  inline void to_screen(fp16 origin_x, fp16 origin_y, fp16 scaling, 
                        int &sx, int &sy) const {
    sx = mul_fp16_fp16_int(x - origin_x, scaling);
    sy = -mul_fp16_fp16_int(y - origin_y, scaling);
  }

  /**
   * Transforms screen coordinates to point coordinates using the specified
   * view.
   *
   * @param x the x screen coordinate
   * @param y the y screen coordinate
   * @param view the current view
   */
  inline void from_screen(int sx, int sy, FigureView *view) {
    from_screen(sx, sy,
                view->get_origin_x(), view->get_origin_y(), 
                view->get_scaling());
  }

  /**
   * Transforms screen coordinates to point coordinates using the specified
   * view parameters.
   *
   * @param x the x screen coordinate
   * @param y the y screen coordinate
   * @param origin_x the origin of x screen coordinate axis
   * @param origin_y the origin of y screen coordinate axis
   * @param scaling the scaling for the screen coordinate axis
   */
  inline void from_screen(int sx, int sy, 
                          fp16 origin_x, fp16 origin_y, fp16 scaling) {
    x = div_int_fp16u_fp16(sx, scaling) + origin_x;
    y = -div_int_fp16u_fp16(sy, scaling) + origin_y;
  }
};

#endif
