// $Id: Editor.cc,v 1.1 2001-05-06 22:12:08 jle Exp $

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
    tool->handle(event, this);
}
