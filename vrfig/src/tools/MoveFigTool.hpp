// $Id: MoveFigTool.hpp,v 1.4 2001-06-10 18:36:44 jle Exp $

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

#ifndef __MoveFigTool_hpp_INCLUDED__
#define __MoveFigTool_hpp_INCLUDED__

#include "Tool.hpp"

/**
 * The MoveFigTool is used to move the figure around the view.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.4 $
 */
class MoveFigTool : public Tool {

protected:

  /** Whether currently moving the figure */
  bool moving;

  /** The original screen coordinates */
  int screen_coord[2];

  /** The original figure origin coordinates */
  fp16 fig_coord[2];

public:

  inline MoveFigTool(): moving(false) {}

  virtual const char *get_name() const;
  
  void draw_icon(int x, int y, int w, int h) const;

  void deactivated(FigureView *view);
  
  int handle(int event, FigureView *view);
};

#endif
