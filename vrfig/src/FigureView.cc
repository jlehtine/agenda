// $Id: FigureView.cc,v 1.3 2001-05-20 11:49:43 jle Exp $

#include <vector.h>
#include <FL/fl_draw.H>
#include <FL/Enumerations.H>
#include <FL/Fl_Window.H>
#include "FigureView.hpp"
#include "Element.hpp"

FigureView::FigureView(int x, int y, int w, int h): 
  Fl_Widget(x, y, w, h), figure(0),
  origin_x(0), origin_y(0), scaling(0x10000) {}

void FigureView::set_figure(Figure *_figure) {
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
