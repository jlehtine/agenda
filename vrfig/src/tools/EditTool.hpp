// $Id: EditTool.hpp,v 1.2 2001-05-23 12:47:52 jle Exp $

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

#ifndef __EditTool_hpp_INCLUDED__
#define __EditTool_hpp_INCLUDED__

#include "Tool.hpp"
#include "Controllable.hpp"

/**
 * Tool for controlling the existing elements.
 *
 * @author Johannes Lehtinen <johannes.lehtine@iki.fi>
 * @version $Revision: 1.2 $
 */
class EditTool : public Tool {

protected:

  /** The element being currently controlled or null if none */
  Controllable *element;

  /** Index of the control point being controlled */
  int cp_index;

  /** The original x coordinate for the control point */
  fp16 org_x;

  /** The original y coordinate for the control point */
  fp16 org_y;

  /** The screen x coordinate of the starting point */
  int sx;

  /** The screen y coordinate of the starting point */
  int sy;

public:

  /** Constructs a new EditTool */
  inline EditTool(): element(0) {}

  virtual const char *get_name() const;

  virtual void draw_icon(int x, int y, int w, int h) const;

  virtual void deactivated(FigureView *view);

  virtual void draw(FigureView *view);

  virtual int handle(int event, FigureView *view);
};

#endif
