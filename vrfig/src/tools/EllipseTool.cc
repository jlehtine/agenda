// $Id: EllipseTool.cc,v 1.3 2001-05-26 16:13:42 jle Exp $

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

#include <FL/Fl.H>
#include <FL/Enumerations.H>
#include <FL/Fl_Bitmap.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include "EllipseTool.hpp"
#include "Action.hpp"
#include "icons/ellipse_icon.xbm"

static Fl_Bitmap ellipse_bitmap
(ellipse_icon_bits, ellipse_icon_width, ellipse_icon_height);

/**
 * An action for undoing ellipses.
 */
class EllipseAction : public Action {

protected:

  /** The view */
  FigureView *view;

  /** The ellipse added */
  Ellipse *element;

public:

  inline EllipseAction(FigureView *view, Ellipse *element):
    view(view), element(element) {}

  void undo() {
    view->remove_element(element);
    delete element;
  }
};

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
    view->get_action_buffer()->add_action(new EllipseAction(view, ellipse));
    view->add_element(ellipse);
    ellipse = 0;
    return 1;

  default:
    return 0;
  }
}
