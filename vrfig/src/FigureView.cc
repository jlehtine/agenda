// $Id: FigureView.cc,v 1.1 2001-05-06 22:12:09 jle Exp $

#include <FL/fl_draw.H>
#include <FL/Enumerations.H>
#include "FigureView.hpp"

FigureView::FigureView(int x, int y, int w, int h): 
  Fl_Widget(x, y, w, h), figure(0), used_cp_style(CP_NONE),
  origin_x(0), origin_y(0), scaling(0x10000) {}

void FigureView::set_figure(Figure *_figure) {
  figure = _figure;
  origin_x = 0;
  origin_y = 0;
  scaling = 0x10000;
}

void FigureView::draw() {
  
  // Empty the screen
  fl_color(FL_WHITE);
  fl_rectf(x(), y(), w(), h());
}
