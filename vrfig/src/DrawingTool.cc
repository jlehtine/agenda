// $Id: DrawingTool.cc,v 1.1 2001-05-06 22:12:09 jle Exp $

#include <X11/Xlib.h>
#include <FL/Fl.H>
#include <FL/Enumerations.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Window.H>
#include <FL/x.H>
#include "DrawingTool.hpp"

static void draw_xor_line(int x1, int y1, int x2, int y2);
static void draw_xor_point(int x, int y);

void DrawingTool::draw(FigureView *view) {
  if (drawing) {
    fl_color(FL_WHITE);
    vector<int>::iterator i = points.begin();
    i += 2;
    while (i <= points.end()) {
      if (i - points.begin() > 2)
        draw_xor_point(*(i-2), *(i-1));
      draw_xor_line(*(i-2), *(i-1), *i, *(i+1));
      i += 2;
    }
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
    }
    break;

  case FL_RELEASE:
    drawing = false;
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

  }
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
