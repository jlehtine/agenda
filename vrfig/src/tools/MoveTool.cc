// $Id: MoveTool.cc,v 1.4 2001-05-23 07:54:36 jle Exp $

#include <vector.h>
#include <FL/Fl.H>
#include <FL/Enumerations.H>
#include <FL/Fl_Bitmap.H>
#include <FL/Fl_Window.H>
#include "MoveTool.hpp"
#include "icons/move_icon.xbm"

#define SELECT_DIST_SQR 16

static Fl_Bitmap move_bitmap
(move_icon_bits, move_icon_width, move_icon_height);

const char *MoveTool::get_name() const {
  static const char *name = "move";
  return name;
}

void MoveTool::draw_icon(int x, int y, int w, int h) const {
  move_bitmap.draw(x, y, w, h,
                   (move_bitmap.w - w) >> 1,
                   (move_bitmap.h - h) >> 1);
}

void MoveTool::deactivated(FigureView *view) {
  element = 0;
}

void MoveTool::draw(FigureView *view) {
  const vector<Element *> *elements = view->get_figure()->get_elements();
  vector<Element *>::const_iterator i = elements->begin();
  while (i < elements->end()) {
    Selectable *sel = dynamic_cast<Selectable *>(*i);
    Movable *mov = dynamic_cast<Movable *>(*(i++));
    if (sel && mov)
      sel->draw_select_helpers(view->get_origin_x(), view->get_origin_y(),
                               view->get_scaling(), true);
  }
}

int MoveTool::handle(int event, FigureView *view) {

  switch (event) {

  case FL_PUSH:
    if (element)
      return 0;

    // Select the element closest to the selection point, if close enough
    do {
      last_x = Fl::event_x();
      last_y = Fl::event_y();
      fp16 x = screen_to_coord(
        last_x, view->get_origin_x(), view->get_scaling());
      fp16 y = screen_to_coord(
        last_y, view->get_origin_y(), view->get_scaling());
      u_fp32 min_dist = ~static_cast<u_fp32>(0);
      vector<Element *> *elements = view->get_figure()->get_elements();
      vector<Element *>::iterator i = elements->begin();
      while (i < elements->end()) {
        Element *elem = *(i++);
        Selectable *sel = dynamic_cast<Selectable *>(elem);
        Movable *mov = dynamic_cast<Movable *>(elem);
        if (sel && mov) {
          u_fp32 d = sel->select_distance_sqr(x, y);
          if (d < min_dist) {
            min_dist = d;
            element = mov;
          }
        }
      }
      if (element) {
        int screen_dist_sqr = mul_fp32_fp32_int(
          min_dist, 
          mul_fp16_fp16_fp32(view->get_scaling(), view->get_scaling()));
        if (screen_dist_sqr > SELECT_DIST_SQR)
          element = 0;
      }
    } while (0);
    return 1;

  case FL_DRAG:
    if (!element)
      return 0;
    do {
      view->window()->make_current();
      int x = Fl::event_x();
      int y = Fl::event_y();
      Element *elem = dynamic_cast<Element *>(element);
      Selectable *sel = dynamic_cast<Selectable *>(element);
      elem->draw(view->get_origin_x(), view->get_origin_y(),
                 view->get_scaling(), true);
      sel->draw_select_helpers(view->get_origin_x(), view->get_origin_y(),
                               view->get_scaling(), true);
      element->move(screen_to_coord(x - last_x, 0, view->get_scaling()),
                    screen_to_coord(y - last_y, 0, view->get_scaling()));
      elem->draw(view->get_origin_x(), view->get_origin_y(),
                 view->get_scaling(), true);
      sel->draw_select_helpers(view->get_origin_x(), view->get_origin_y(),
                               view->get_scaling(), true);
      last_x = x;
      last_y = y;
    } while (0);
    return 1;
    
  case FL_RELEASE:
    if (!element)
      return 0;
    element = 0;
    view->redraw();
    return 1;

  default:
    return 0;
  }
}
