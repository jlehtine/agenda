// $Id: Editor.hpp,v 1.4 2001-05-23 12:47:50 jle Exp $

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

#ifndef __Editor_hpp_INCLUDED__
#define __Editor_hpp_INCLUDED__

#include "FigureView.hpp"
#include "Tool.hpp"

/**
 * The widget used for the editing area. This class is responsible for
 * rendering the elements, passing user activities to tools and calling
 * the tool specific drawing hooks.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.4 $
 */
class Editor : public FigureView {

protected:

  /** The current tool */
  Tool *tool;

public:

  /**
   * Constructs a new editor and initializes it for default look. No
   * figure or callback hooks are associated with the editor.
   */
  Editor(int x, int y, int w, int h);

  virtual void set_figure(Figure *figure);

  /**
   * Sets the current tool.
   *
   * @param tool the new tool to be used
   */
  void set_tool(Tool *_tool);

  void set_origin(fp16 x, fp16 y);

  void set_scaling(u_fp16 _scaling);

  /**
   * Draws the widget.
   */
  void draw();

  /**
   * Handle the events.
   */
  int handle(int event);
};

#endif
