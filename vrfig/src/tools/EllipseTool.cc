// $Id: EllipseTool.cc,v 1.1 2001-05-23 07:55:00 jle Exp $

#include <FL/Fl.H>
#include <FL/Enumerations.H>
#include <FL/Fl_Bitmap.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include "EllipseTool.hpp"
#include "icons/ellipse_icon.xbm"

static Fl_Bitmap ellipse_bitmap
(ellipse_icon_bits, ellipse_icon_width, ellipse_icon_height);

const char *EllipseTool::get_name() const {
  static const char *name = "ellipse";
  return name;
}

void EllipseTool::draw_icon(int x, int y, int w, int h) const {
  ellipse_bitmap.draw(x, y, w, h,
                      (ellipse_bitmap.w - w) >> 1,
                      (ellipse_bitmap.h - h) >> 1);
}

void EllipseTool::deactivated(FigureView *view) {
  ellipse = 0;
}

void EllipseTool::draw(FigureView *view) {
  if (ellipse) {
    ellipse->draw(view->get_origin_x(), view->get_origin_y(), 
                  view->get_scaling(), true);
  }
}

int EllipseTool::handle(int event, FigureView *view) {

  switch (event) {

  case FL_PUSH:
    if (ellipse)
      return 1;
    do {
      view->window()->make_current();
      fp16 x = screen_to_coord(Fl::event_x(), view->get_origin_x(),
                               view->get_scaling());
      fp16 y = screen_to_coord(Fl::event_y(), view->get_origin_y(),
                               view->get_scaling());
      ellipse = new Ellipse(x, y, x, y);
      draw(view);
    } while (0);
    return 1;

  case FL_DRAG:
    if (!ellipse)
      return 1;
    view->window()->make_current();
    draw(view);
    ellipse->control(1,
                     screen_to_coord(Fl::event_x(), view->get_origin_x(),
                                     view->get_scaling()),
                     screen_to_coord(Fl::event_y(), view->get_origin_y(),
                                     view->get_scaling()));
    draw(view);
    return 1;

  case FL_RELEASE:
    if (!ellipse)
      return 1;
    view->add_element(ellipse);
    ellipse = 0;
    return 1;

  default:
    return 0;
  }
}
