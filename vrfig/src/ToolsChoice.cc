// $Id: ToolsChoice.cc,v 1.1 2001-05-17 19:35:28 jle Exp $

#include <FL/Enumerations.H>
#include <FL/fl_draw.h>
#include "ToolsChoice.hpp"

#define CELL_SIZE 16

ToolsChoice::ToolsChoice(int x, int y, vector<Tool *> *tools):
  Fl_Window(x, y, CELL_SIZE, CELL_SIZE, "Tool choice"), tools(tools) {

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
  if (w_cells * h_cells < numtools)
    h_cells++;

  // Set the dimensions for the window
  resize(ToolsChoice::x(), ToolsChoice::y(), 
         w_cells * CELL_SIZE, h_cells * CELL_SIZE);
}

void ToolsChoice::draw() {
  
  // Clear the window
  fl_color(color());
  fl_rectf(x(), y(), w(), h());

  // Draw the tool icons
  fl_color(labelcolor());
  unsigned int x, y;
  unsigned int tool_i = 0;
  for (y = 0; y < h_cells && tool_i < tools->size(); y++) {
    for (x = 0; x < w_cells && tool_i < tools->size(); x++, tool_i++) {
      ((*tools)[tool_i])->draw_icon(x * CELL_SIZE,
                                    y * CELL_SIZE,
                                    CELL_SIZE, CELL_SIZE);
    }
  }
}

int ToolsChoice::handle(int event) {
  return 0;
}
