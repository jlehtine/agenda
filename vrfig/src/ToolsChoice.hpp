// $Id: ToolsChoice.hpp,v 1.3 2001-05-23 12:47:51 jle Exp $

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

#ifndef __ToolsChoice_hpp_INCLUDED__
#define __ToolsChoice_hpp_INCLUDED__

#include <FL/Fl_Window.H>
#include <vector.h>
#include <math.h>
#include "Tool.hpp"

/**
 * Provides the menu for selecting the active tool.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.3 $
 */
class ToolsChoice : public Fl_Window {

protected:

  /** Width of the menu in cells */
  unsigned int w_cells;

  /** Height of the menu in cells */
  unsigned int h_cells;

  /** The available tools */
  vector <Tool *> *tools;

  /** The currently selected tool */
  Tool *selected_tool;

  /** Whether waiting for button release */
  bool waiting_release;
  
public:

  ToolsChoice(int x, int y, vector<Tool *> *tools);

  void draw();

  int handle(int event);

  /**
   * Returns the currently selected tool.
   *
   * @return the currently selected tool
   */
  inline Tool *get_tool() const {
    return selected_tool;
  }

  /**
   * Sets the selected tool.
   *
   * @param tool the selected tool
   */
  inline void set_tool(Tool *tool) {
    selected_tool = tool;
  }
};

#endif
