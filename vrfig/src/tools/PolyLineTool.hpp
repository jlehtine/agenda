// $Id: PolyLineTool.hpp,v 1.3 2001-05-23 12:47:52 jle Exp $

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
 * @version $Revision: 1.3 $
 */
class PolyLineTool : public Tool {

protected:

  /** The last polyline drawn or 0 for none */
  PolyLine *last_polyline;

  /** Whether currently drawing a line segment */
  bool drawing_segment;

  /** The x coordinate of the last fixed point */
  int last_x;

  /** The y coordinate of the last fixed point */
  int last_y;

  /** The last end point x coordinate for the segment */
  int seg_x;

  /** The last end point y coordinate for the segment */
  int seg_y;

public:

  PolyLineTool(): last_polyline(0), drawing_segment(false) {}

  virtual const char *get_name() const;

  virtual void draw_icon(int x, int y, int w, int h) const;

  virtual void deactivated(FigureView *view);

  virtual void draw(FigureView *view);

  virtual int handle(int event, FigureView *view);
};

#endif
