// $Id: EditTool.cc,v 1.4 2001-05-24 19:49:58 jle Exp $

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
#include <FL/fl_draw.H>
#include "EditTool.hpp"
#include "Selectable.hpp"
#include "icons/edit_icon.xbm"
#include "flext.hpp"

static Fl_Bitmap edit_bitmap
(edit_icon_bits, edit_icon_width, edit_icon_height);

static void draw_control_points(Controllable *element, FigureView *view);

const char *EditTool::get_name() const {
  static const char *name = "edit";
  return name;
}

void EditTool::draw_icon(int x, int y, int w, int h) const {
  edit_bitmap.draw(x, y, w, h,
                   (edit_bitmap.w - w) >> 1,
                   (edit_bitmap.h - h) >> 1);
}

void EditTool::deactivated(FigureView *view) {
  element = 0;
}

void EditTool::draw(FigureView *view) {

  // Draw the control points for the controllable elements
  const vector<Element *> *elements = view->get_figure()->get_elements();
  vector<Element *>::const_iterator i = elements->begin();
  fl_color(FL_WHITE);
  int old_func = fle_xorred_mode();
  while (i < elements->end()) {
    Controllable *elem = dynamic_cast<Controllable *>(*(i++));
    draw_control_points(elem, view);
  }
  fle_reset_mode(old_func);
}

int EditTool::handle(int event, FigureView *view) {

  switch (event) {

  case FL_PUSH:
    if (element)
      return 1;
    do {
      view->window()->make_current();
    
      // Select the closest control point (if it is close enough)
      sx = Fl::event_x();
      sy = Fl::event_y();
      fp16 x = screen_to_coord(
        sx, view->get_origin_x(), view->get_scaling());
      fp16 y = screen_to_coord(
        sy, view->get_origin_y(), view->get_scaling());
      u_fp32 min_dist = ~static_cast<u_fp32>(0);
      const vector<Element *> *elements = view->get_figure()->get_elements();
      vector<Element *>::const_iterator i = elements->begin();
      while (i < elements->end()) {
        Controllable *elem = dynamic_cast<Controllable *>(*(i++));
        if (elem) {
          const vector<fp16> *points = elem->get_control_points();
          vector<fp16>::const_iterator ii = points->begin();
          int index = 0;
          while (ii < points->end()) {
            fp16 cx = *(ii++);
            fp16 cy = *(ii++);
            u_fp32 d = vector_length_sqr_fp16_fp32(x - cx, y - cy);
            if (d < min_dist) {
              min_dist = d;
              element = elem;
              cp_index = index;
              org_x = cx;
              org_y = cy;
            }
            index++;
          }
        }
      }
    
      // Check if the closest control point is close enough
      if (element) {
        int screen_dist_sqr = mul_fp32_fp32_int(
          min_dist, 
          mul_fp16_fp16_fp32(view->get_scaling(), view->get_scaling()));
        if (screen_dist_sqr > VRF_DEFAULT_SELECT_DIST_SQR)
          element = 0;
      }

      // Hide the control points for the element being controlled
      if (element) {
        fl_color(FL_WHITE);
        int old_func = fle_xorred_mode();
        draw_control_points(element, view);
        fle_reset_mode(old_func);
      }
      
    } while (0);
    return 1;

  case FL_DRAG:
    if (!element)
      return 1;
    do {
      view->window()->make_current();

      // Hide the element
      Element *elem = dynamic_cast<Element *>(element);
      elem->draw(view->get_origin_x(), view->get_origin_y(), 
                 view->get_scaling(), true);

      // Edit the control point
      fp16 new_x = org_x + screen_to_coord(
        Fl::event_x() - sx, 0, view->get_scaling());
      fp16 new_y = org_y + screen_to_coord(
        Fl::event_y() - sy, 0, view->get_scaling());
      element->control(cp_index, new_x, new_y);

      // Show the element again
      elem->draw(view->get_origin_x(), view->get_origin_y(), 
                 view->get_scaling(), true);

    } while (0);
    return 1;

  case FL_RELEASE:
    if (!element)
      return 1;
    element = 0;
    view->redraw();
    return 1;
    
  default:
    return 0;
  }
}

static void draw_control_points(Controllable *element, FigureView *view) {
  const vector<fp16> *points = element->get_control_points();
  vector<fp16>::const_iterator i = points->begin();
  while (i < points->end()) {
    fp16 x = *(i++);
    fp16 y = *(i++);
    Selectable::draw_select_helper(
      x, y, view->get_origin_x(), view->get_origin_y(), view->get_scaling());
  }
}
