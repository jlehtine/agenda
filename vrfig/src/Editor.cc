// $Id: Editor.cc,v 1.4 2001-05-20 11:49:43 jle Exp $

#include "Editor.hpp"

Editor::Editor(int x, int y, int w, int h): 
  FigureView(x, y, w, h), tool(0) {}

void Editor::set_figure(Figure *figure_) {
  if (tool && figure)
    tool->deactivated(this);
  FigureView::set_figure(figure_);
  if (tool && figure)
    tool->activated(this);
}

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
