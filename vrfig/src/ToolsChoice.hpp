// $Id: ToolsChoice.hpp,v 1.1 2001-05-17 19:35:28 jle Exp $

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
 * @version $Revision: 1.1 $
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
