// $Id: DeleteTool.cc,v 1.4 2001-05-23 12:47:51 jle Exp $

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
#include <FL/Fl_Bitmap.H>
#include "DeleteTool.hpp"
#include "Selectable.hpp"
#include "icons/delete_icon.xbm"

#define SELECT_DIST_SQR 16

static Fl_Bitmap delete_bitmap
(delete_icon_bits, delete_icon_width, delete_icon_height);

const char *DeleteTool::get_name() const {
  static const char *name = "delete";
  return name;
}

void DeleteTool::draw_icon(int x, int y, int w, int h) const {
  delete_bitmap.draw(x, y, w, h,
                   (delete_bitmap.w - w) >> 1,
                   (delete_bitmap.h - h) >> 1);
}

void DeleteTool::draw(FigureView *view) {
  const vector<Element *> *elements = view->get_figure()->get_elements();
  vector<Element *>::const_iterator i = elements->begin();
  while (i < elements->end()) {
    Selectable *sel = dynamic_cast<Selectable *>(*(i++));
    if (sel)
      sel->draw_select_helpers(view->get_origin_x(), view->get_origin_y(),
                               view->get_scaling(), true);
  }
}

int DeleteTool::handle(int event, FigureView *view) {
  
  switch (event) {

  case FL_PUSH:
    
    // Find the closest element
    // Select the element closest to the selection point, if close enough
    do {
      fp16 x = screen_to_coord(
        Fl::event_x(), view->get_origin_x(), view->get_scaling());
      fp16 y = screen_to_coord(
        Fl::event_y(), view->get_origin_y(), view->get_scaling());
      u_fp32 min_dist = ~static_cast<u_fp32>(0);
      vector<Element *> *elements = view->get_figure()->get_elements();
      vector<Element *>::iterator i = elements->begin();
      vector<Element *>::iterator closest = 0;
      while (i < elements->end()) {
        Element *elem = *i;
        Selectable *sel = dynamic_cast<Selectable *>(elem);
        if (sel) {
          u_fp32 d = sel->select_distance_sqr(x, y);
          if (d < min_dist) {
            min_dist = d;
            closest = i;
          }
        }
        i++;
      }
      if (closest) {
        int screen_dist_sqr = mul_fp32_fp32_int(
          min_dist, 
          mul_fp16_fp16_fp32(view->get_scaling(), view->get_scaling()));
        if (screen_dist_sqr <= SELECT_DIST_SQR) {
          
          // Delete the element
          elements->erase(closest);
          view->redraw();
        }
      }
    } while (0);
    return 1;

  default:
    return 0;
  }
}
