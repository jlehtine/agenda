// $Id: DrawingTool.hpp,v 1.2 2001-05-23 12:47:51 jle Exp $

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

#ifndef __DrawingTool_hpp_INCLUDED__
#define __DrawingTool_hpp_INCLUDED__

#if USE_EXPERIMENTAL_UI

#include <vector>
#include "Tool.hpp"

/**
 * A common base class for tools that draw new elements.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.2 $
 */
class DrawingTool : public Tool {

protected:

  /** The recorded points */
  vector<int> points;

  /** Whether we are currently drawing object */
  bool drawing;
  
  /**
   * Draws the current line as xorred line.
   *
   * @param view the view to use for drawing
   */
  void draw_current_line(FigureView *view);

public:

  /**
   * Constructs a new tool.
   */
  inline DrawingTool(): points(), drawing(false) {}

  virtual ~DrawingTool() {}

  // Doc inherited from Tool
  void deactivated(FigureView *view);

  /**
   * If the user was drawing, draw the recorded points. Otherwise
   * do nothing.
   *
   * @param view the current view
   */
  virtual void draw(FigureView *view);
  
  // Doc inherited from Tool
  virtual int handle(int event, FigureView *view);
};

#endif

#endif
