// $Id: RectangleTool.cc,v 1.3 2001-05-28 18:46:26 jle Exp $

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
#include <FL/Fl_Window.H>
#include <FL/Fl_Bitmap.H>
#include <FL/fl_draw.H>
#include "RectangleTool.hpp"
#include "elements/Rectangle.hpp"
#include "icons/rect_icon.xbm"
#include "flext.hpp"
#include "mathutil.hpp"

static Fl_Bitmap rect_bitmap
(rect_icon_bits, rect_icon_width, rect_icon_height);

/**
 * An action for undoing rectangles.
 */
class RectangleAction : public Action {

protected:

  /** The view */
  FigureView *view;

  /** The rectangle added */
  Rectangle *element;

public:

  inline RectangleAction(FigureView *view, Rectangle *element):
    view(view), element(element) {}

  void undo() {
    view->remove_element(element);
    delete element;
  }
};

const char *RectangleTool::get_name() const {
  static const char *name = "rectangle";
  return name;
}

void RectangleTool::draw_icon(int x, int y, int w, int h) const {
  rect_bitmap.draw(x, y, w, h,
                   (rect_bitmap.w - w) >> 1,
                   (rect_bitmap.h - h) >> 1);
}

void RectangleTool::deactivated(FigureView *view) {
  drawing = false;
}

void RectangleTool::draw(FigureView *view) {
  if (drawing) {

    // Select the correct dimensions
    int x, y, w, h;
    if (xs < last_x) {
      x = xs;
      w = last_x - xs;
    } else {
      x = last_x;
      w = xs - last_x;
    }
    if (ys < last_y) {
      y = ys;
      h = last_y - ys;
    } else {
      y = last_y;
      h = ys - last_y;
    }

    // Draw the rectangle
    fl_color(FL_WHITE);
    int old_func = fle_xorred_mode();
    fl_rect(x, y, w, h);
    fle_reset_mode(old_func);
  }
}

int RectangleTool::handle(int event, FigureView *view) {

  switch (event) {

  case FL_PUSH:
    if (drawing)
      return 1;
    view->window()->make_current();
    xs = Fl::event_x();
    ys = Fl::event_y();
    last_x = xs;
    last_y = ys;
    drawing = true;
    draw(view);
    return 1;

  case FL_DRAG:
    if (!drawing)
      return 1;
    view->window()->make_current();
    draw(view);
    last_x = Fl::event_x();
    last_y = Fl::event_y();
    draw(view);
    return 1;

  case FL_RELEASE:
    if (!drawing)
      return 1;
    do {
      int x, y, w, h;
      if (xs < last_x) {
        x = xs;
        w = last_x - xs;
      } else {
        x = last_x;
        w = xs - last_x;
      }
      if (ys < last_y) {
        y = ys;
        h = last_y - ys;
      } else {
        y = last_y;
        h = ys - last_y;
      }
      fp16 fx = screen_to_coord(x, view->get_origin_x(), view->get_scaling());
      fp16 fy = screen_to_coord(y, view->get_origin_y(), view->get_scaling());
      fp16 fw = screen_to_coord(w, 0, view->get_scaling());
      fp16 fh = screen_to_coord(h, 0, view->get_scaling());
      Rectangle *rect = new Rectangle(fx, fy, fw, fh);
      view->add_element(rect);
      view->redraw();
      drawing = false;

      view->get_action_buffer()->add_action(
        new RectangleAction(view, rect));
    } while (0);
    return 1;

  default:
    return 0;
  }
}
