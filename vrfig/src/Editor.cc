// $Id: Editor.cc,v 1.7 2001-05-23 12:47:49 jle Exp $

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

void Editor::set_origin(fp16 x, fp16 y) {
  if (tool)
    tool->deactivated(this);
  FigureView::set_origin(x, y);
  if (tool)
    tool->activated(this);
}

void Editor::set_scaling(u_fp16 _scaling) {
  if (tool)
    tool->deactivated(this);
  FigureView::set_scaling(_scaling);
  if (tool)
    tool->activated(this);
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
