// $Id: Controllable.hpp,v 1.3 2001-05-29 18:05:10 jle Exp $

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

#ifndef __Controllable_hpp_INCLUDED__
#define __Controllable_hpp_INCLUDED__

#include "FigureView.hpp"
#include "Selectable.hpp"
#include "mathutil.hpp"

/**
 * An interface for elements that can be controlled by control points.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.3 $
 */
class Controllable {

public:

  /**
   * Returns a list of control points.
   *
   * @return a list of control points
   */
  virtual const vector<Point> *get_control_points() const = 0;

  /**
   * Moves the specified control point to the specified location.
   * Because of element specific restrictions the control point might
   * not move exactly to the point pointed by the user. The control
   * points should be read after the move operation to find out where
   * the point actually moved.
   *
   * @param i the index of control point to move
   * @param x the new x coordinate for the control point
   * @param y the new y coordinate for the control point
   */
  virtual void control(unsigned int i, fp16 x, fp16 y) = 0;
};

#endif
