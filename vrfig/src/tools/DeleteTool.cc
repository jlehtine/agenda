// $Id: DeleteTool.cc,v 1.8 2001-06-10 18:36:44 jle Exp $

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
#include "Action.hpp"
#include "Point.hpp"
#include "icons/delete_icon.xbm"

static Fl_Bitmap delete_bitmap
(delete_icon_bits, delete_icon_width, delete_icon_height);

/**
 * An action for undoing delete operations.
 */
class DeleteAction : public Action {
  
protected:

  /** The deleted element */
  Element *element;

  /** The figure view */
  FigureView *view;

public:

  inline DeleteAction(Element *element, FigureView *view): 
    element(element), view(view) {}

  virtual void undo() {
    view->add_element(element);
  }

  virtual void commit() {
    delete element;
  }
};

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
      Point p;
      p.from_screen(Fl::event_x(), Fl::event_y(), view);
      u_fp32 min_dist = ~static_cast<u_fp32>(0);
      vector<Element *> *elements = view->get_figure()->get_elements();
      vector<Element *>::iterator i = elements->begin();
      vector<Element *>::iterator closest = 0;
      while (i < elements->end()) {
        Element *elem = *i;
        Selectable *sel = dynamic_cast<Selectable *>(elem);
        if (sel) {
          u_fp32 d = sel->select_distance_sqr(p.x, p.y);
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
        if (screen_dist_sqr <= VRF_DEFAULT_SELECT_DIST_SQR) {
          
          // Create an action record
          DeleteAction *action = new DeleteAction(*closest, view);

          // Delete the element
          view->remove_element(*closest);
          view->get_action_buffer()->add_action(action);
        }
      }
    } while (0);
    return 1;

  default:
    return 0;
  }
}
