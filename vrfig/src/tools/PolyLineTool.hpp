// $Id: PolyLineTool.hpp,v 1.4 2001-05-27 13:21:46 jle Exp $

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

#ifndef __PolyLineTool_hpp_INCLUDED__
#define __PolyLineTool_hpp_INCLUDED__

#include "Tool.hpp"
#include "elements/PolyLine.hpp"

/**
 * Tool for drawing polylines and polygons.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.4 $
 */
class PolyLineTool : public Tool {

protected:

  /** The polyline being currently extended */
  PolyLine *polyline;

  /** Whether the point will be added to the start of the polyline */
  bool extend_first;

  /** The x screen coordinate of the start point */
  int start_x;

  /** The y screen coordinate of the start point */
  int start_y;

  /** The last end point x screen coordinate */
  int last_x;

  /** The last end point y screen coordinate */
  int last_y;

  /** Whether drawing a new polyline */
  bool new_polyline;

public:

  PolyLineTool(): polyline(0) {}

  virtual const char *get_name() const;

  virtual void draw_icon(int x, int y, int w, int h) const;

  virtual void deactivated(FigureView *view);

  virtual void draw(FigureView *view);

  virtual int handle(int event, FigureView *view);
};

#endif
