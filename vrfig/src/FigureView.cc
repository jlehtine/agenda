// $Id: FigureView.cc,v 1.5 2001-05-26 14:25:19 jle Exp $

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

#include <vector.h>
#include <FL/fl_draw.H>
#include <FL/Enumerations.H>
#include <FL/Fl_Window.H>
#include "FigureView.hpp"
#include "Element.hpp"

FigureView::FigureView(int x, int y, int w, int h, ActionBuffer *abuffer): 
  Fl_Widget(x, y, w, h), figure(0),
  origin_x(0), origin_y(0), scaling(0x10000),
  action_buffer(abuffer) {}

void FigureView::set_figure(Figure *_figure) {
  action_buffer->clear();
  figure = _figure;
  origin_x = 0;
  origin_y = 0;
  scaling = 0x10000;
  redraw();
}

void FigureView::set_scaling(unsigned int _scaling) {
  scaling = _scaling;
  redraw();
}

void FigureView::set_origin(int x, int y) {
  origin_x = x;
  origin_y = y;
  redraw();
}

void FigureView::draw() {
  
  // Empty the screen
  fl_color(FL_WHITE);
  fl_rectf(x(), y(), w(), h());

  // Draw the elements
  if (figure != 0) {
    fl_push_clip(x(), y(), w(), h());
    vector<Element *> *elements = figure->get_elements();
    vector<Element *>::iterator i = elements->begin();
    while (i < elements->end()) {
      (*i)->draw(origin_x, origin_y, scaling);
      i++;
    }
    fl_pop_clip();
  }
}

void FigureView::add_element(Element *element) {
  figure->add_element(element);
  window()->make_current();
  fl_push_clip(x(), y(), w(), h());
  element->draw(origin_x, origin_y, scaling);
  fl_pop_clip();
}
