// $Id: MoveTool.hpp,v 1.3 2001-05-27 11:31:30 jle Exp $

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

#ifndef __MoveTool_hpp_INCLUDED__
#define __MoveTool_hpp_INCLUDED__

#include "Tool.hpp"
#include "Movable.hpp"
#include "Selectable.hpp"
#include "Figure.hpp"

/**
 * This tool can move elements which implement Selectable and Movable
 * interfaces.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.3 $
 */
class MoveTool : public Tool {
  
protected:

  /** The element currently being moved or null if none */
  Movable *element;
  
  /** The last x coordinate for the mouse */
  int last_x;

  /** The last y coordinate for the mouse */
  int last_y;

  /** The cumulative x movement */
  fp16 cum_x;

  /** The cumulative y movement */
  fp16 cum_y;

public:

  MoveTool(): element(0) {}

  virtual const char *get_name() const;

  virtual void draw_icon(int x, int y, int w, int h) const;

  virtual void deactivated(FigureView *view);

  virtual void draw(FigureView *view);

  virtual int handle(int event, FigureView *view);
};

#endif
