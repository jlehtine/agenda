// $Id: Editor.cc,v 1.2 2001-05-07 21:02:10 jle Exp $

#include "Editor.hpp"

Editor::Editor(int x, int y, int w, int h): 
  FigureView(x, y, w, h), tool(0) {}

void Editor::set_tool(Tool *_tool) {
  if (tool != 0)
    tool->deactivated(this);
  tool = _tool;
  if (tool != 0)
    tool->activated(this);
  redraw();
}

void Editor::draw() {
  FigureView::draw();
  if (tool != 0)
    tool->draw(this);
}

int Editor::handle(int event) {
  if (tool != 0)
    return tool->handle(event, this);
  else
    return 0;
}
