// $Id: RectangleTool.hpp,v 1.2 2001-05-23 12:47:52 jle Exp $

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

#ifndef __RectangleTool_hpp_INCLUDED__
#define __RectangleTool_hpp_INCLUDED__

#include "Tool.hpp"

/**
 * A tool for drawing rectangles.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.2 $
 */
class RectangleTool : public Tool {

protected:

  /** Whether current drawing a rectangle */
  bool drawing;

  /** The starting x screen coordinate */
  int xs;

  /** The starting y screen coordinate */
  int ys;

  /** The last x screen coordinate */
  int last_x;
  
  /** The last y screen coordinate */
  int last_y;

public:

  /** Constructs a new RectangleTool */
  inline RectangleTool(): drawing(false) {}

  virtual const char *get_name() const;

  virtual void draw_icon(int x, int y, int w, int h) const;

  virtual void deactivated(FigureView *view);

  virtual void draw(FigureView *view);

  virtual int handle(int event, FigureView *view);
  
};

#endif
