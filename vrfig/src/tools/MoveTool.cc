// $Id: MoveTool.cc,v 1.7 2001-05-27 11:31:30 jle Exp $

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
#include <FL/Fl.H>
#include <FL/Enumerations.H>
#include <FL/Fl_Bitmap.H>
#include <FL/Fl_Window.H>
#include "MoveTool.hpp"
#include "Action.hpp"
#include "icons/move_icon.xbm"

static Fl_Bitmap move_bitmap
(move_icon_bits, move_icon_width, move_icon_height);

/**
 * An action for undoing movements.
 */
class MoveAction : public Action {

protected:

  /** The view */
  FigureView *view;

  /** The element moved */
  Movable *element;

  /** The x movement */
  fp16 move_x;

  /** The y movement */
  fp16 move_y;

public:

  inline MoveAction(FigureView *view, Movable *element, 
                    fp16 move_x, fp16 move_y):
    view(view), element(element), move_x(move_x), move_y(move_y) {}

  virtual void undo() {
    element->move(-move_x, -move_y);
    view->redraw();
  }
};

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
        if (screen_dist_sqr > VRF_DEFAULT_SELECT_DIST_SQR)
          element = 0;
        else {
          cum_x = 0;
          cum_y = 0;
        }
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
      fp16 move_x = screen_to_coord(x - last_x, view->get_scaling());
      fp16 move_y = screen_to_coord(y - last_y, view->get_scaling());
      element->move(move_x, move_y);
      cum_x += move_x;
      cum_y += move_y;
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
    view->get_action_buffer()->add_action
      (new MoveAction(view, element, cum_x, cum_y));
    element = 0;
    view->redraw();
    return 1;

  default:
    return 0;
  }
}
