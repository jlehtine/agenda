// $Id: ToolsChoice.cc,v 1.6 2001-05-23 12:47:51 jle Exp $

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

#include <FL/Fl.H>
#include <FL/Enumerations.H>
#include <FL/fl_draw.H>
#include "ToolsChoice.hpp"

#define CELL_SIZE 16

ToolsChoice::ToolsChoice(int x, int y, vector<Tool *> *tools):
  Fl_Window(x, y, CELL_SIZE, CELL_SIZE, "Tool choice"), tools(tools),
  waiting_release(false) {

  // Configure the window
  border(0);
  set_modal();
#ifdef FLTK_VR3
  box(VR_BUTTON_UP_BOX);
#else
  box(FL_UP_BOX);
#endif

  // Calculate the size of tool box
  unsigned int numtools = tools->size();
  w_cells = static_cast<int>(sqrt(numtools));
  h_cells = w_cells;
  bool increase_h = true;
  while (w_cells * h_cells < numtools) {
    if (increase_h)
      h_cells++;
    else
      w_cells++;
    increase_h = !increase_h;
  }

  // Set the dimensions for the window
  resize(ToolsChoice::x(), ToolsChoice::y(), 
         w_cells * CELL_SIZE + 4, h_cells * CELL_SIZE + 4);
}

void ToolsChoice::draw() {
  
  // Clear the window
  Fl_Window::draw();

  // Draw the tool icons
  fl_color(labelcolor());
  unsigned int x, y;
  unsigned int tool_i = 0;
  for (y = 0; y < h_cells && tool_i < tools->size(); y++) {
    for (x = 0; x < w_cells && tool_i < tools->size(); x++, tool_i++) {
      ((*tools)[tool_i])->draw_icon(x * CELL_SIZE + 2,
                                    y * CELL_SIZE + 2,
                                    CELL_SIZE, CELL_SIZE);
    }
  }
}

int ToolsChoice::handle(int event) {
  switch (event) {
    int x, y;

  case FL_PUSH:
    
    // Check if coordinates match some tool
    x = (Fl::event_x() - 2) / CELL_SIZE;
    y = (Fl::event_y() - 2) / CELL_SIZE;
    if (x >= 0 || x < static_cast<int>(w_cells)
        || y >= 0 || y < static_cast<int>(h_cells)) {
      unsigned int tool = static_cast<unsigned int>(y) * w_cells 
        + static_cast<unsigned int>(x);
      if (tool < tools->size()) {
        selected_tool = (*tools)[tool];
        fl_color(labelcolor());
        fl_rect(x * CELL_SIZE + 2, y * CELL_SIZE + 2,
                CELL_SIZE, CELL_SIZE);
        waiting_release = true;
        return 1;
      }
    }
    
    // Otherwise just close the window
    hide();
    (callback())(this, user_data());
    return 1;

  case FL_RELEASE:
    
    // Check if we were waiting for release
    if (waiting_release) {
      waiting_release = false;
      hide();
      (callback())(this, user_data());
      return 1;
    } else
      return 0;

  default:
    return 0;
  }
}
