// $Id: DrawingTool.cc,v 1.3 2001-05-08 15:31:19 jle Exp $

#include <X11/Xlib.h>
#include <FL/Fl.H>
#include <FL/Enumerations.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Window.H>
#include <FL/x.H>
#include "DrawingTool.hpp"
#include "Element.hpp"

static void draw_xor_line(int x1, int y1, int x2, int y2);
static void draw_xor_point(int x, int y);

void DrawingTool::draw_current_line(FigureView *view) {
  fl_color(FL_WHITE);
  vector<int>::iterator i = points.begin();
  i += 2;
  while (i < points.end()) {
    if (i - points.begin() > 2)
      draw_xor_point(*(i-2), *(i-1));
    draw_xor_line(*(i-2), *(i-1), *i, *(i+1));
    i += 2;
  }
}

void DrawingTool::deactivated(FigureView *view) {
  if (drawing) {
    view->window()->make_current();
    draw_current_line(view);
  }
  points.clear();
  drawing = false;
}

void DrawingTool::draw(FigureView *view) {
  if (drawing) {
    draw_current_line(view);
  }
}

int DrawingTool::handle(int event, FigureView *view) {
  switch (event) {

  case FL_PUSH:
    if (Fl::event_button() == FL_LEFT_MOUSE && !drawing) {
      drawing = true;
      points.clear();
      points.insert(points.end(), Fl::event_x());
      points.insert(points.end(), Fl::event_y());
      view->window()->make_current();
      fl_color(FL_WHITE);
      draw_xor_point(Fl::event_x(), Fl::event_y());
    }
    break;

  case FL_RELEASE:
    if (drawing) {
      view->window()->make_current();
      draw_current_line(view);
      Element *element = Element::fit_element
        (&points, view->get_origin_x(), view->get_origin_y(),
         view->get_scaling());
      if (element != 0)
        view->add_element(element);
      drawing = false;
      points.clear();
    }
    break;

  case FL_DRAG:
    if (drawing) {
      vector<int>::iterator i = points.end();
      if (*(i-2) != Fl::event_x() || *(i-1) != Fl::event_y()) {
        points.insert(points.end(), Fl::event_x());
        points.insert(points.end(), Fl::event_y());
        i = points.end();
        view->window()->make_current();
        fl_color(FL_WHITE);
        draw_xor_point(*(i-4), *(i-3));
        draw_xor_line(*(i-4), *(i-3), *(i-2), *(i-1));
      }
    }
    break;

  default:
    return 0;

  }
  return 1;
}

static void draw_xor_line(int x1, int y1, int x2, int y2) {
  XGCValues gcValues;
  XGetGCValues(fl_display, fl_gc, GCFunction, &gcValues);
  int old_func = gcValues.function;
  gcValues.function = GXxor;
  XChangeGC(fl_display, fl_gc, GCFunction, &gcValues);
  XDrawLine(fl_display, fl_window, fl_gc, x1, y1, x2, y2);
  gcValues.function = old_func;
  XChangeGC(fl_display, fl_gc, GCFunction, &gcValues);
}

static void draw_xor_point(int x, int y) {
  XGCValues gcValues;
  XGetGCValues(fl_display, fl_gc, GCFunction, &gcValues);
  int old_func = gcValues.function;
  gcValues.function = GXxor;
  XChangeGC(fl_display, fl_gc, GCFunction, &gcValues);
  XDrawPoint(fl_display, fl_window, fl_gc, x, y);
  gcValues.function = old_func;
  XChangeGC(fl_display, fl_gc, GCFunction, &gcValues);
}
