// $Id: Editor.cc,v 1.3 2001-05-17 19:35:27 jle Exp $

#include "Editor.hpp"

Editor::Editor(int x, int y, int w, int h): 
  FigureView(x, y, w, h), tool(0) {}

void Editor::set_tool(Tool *_tool) {
  if (tool)
    tool->deactivated(this);
  tool = _tool;
  if (tool)
    tool->activated(this);
  redraw();
}

void Editor::draw() {
  FigureView::draw();
  if (tool)
    tool->draw(this);
}

int Editor::handle(int event) {
  if (tool)
    return tool->handle(event, this);
  else
    return 0;
}
