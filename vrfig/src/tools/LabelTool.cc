// $Id: LabelTool.cc,v 1.2 2001-05-26 16:13:10 jle Exp $

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
#include "LabelTool.hpp"
#include "elements/Label.hpp"
#include "icons/text_icon.xbm"
#include "vrfig.hpp"

static Fl_Bitmap text_bitmap
(text_icon_bits, text_icon_width, text_icon_height);

const char *LabelTool::get_name() const {
  static const char *name = "label";
  return name;
}

void LabelTool::draw_icon(int x, int y, int w, int h) const {
  text_bitmap.draw(x, y, w, h,
                   (text_bitmap.w - w) >> 1,
                   (text_bitmap.h - h) >> 1);
}

void LabelTool::activated(FigureView *view) {
  Fl::focus(view);
}

void LabelTool::deactivated(FigureView *view) {
  unselect(view);
}

void LabelTool::draw(FigureView *view) {
  if (element)
    draw_xorred_text_cursor(view);
}

int LabelTool::handle(int event, FigureView *view) {

  switch (event) {

  case FL_PUSH:
    do {
      view->window()->make_current();
      unselect(view);

      // Choose the closest textual element if it is close enough
      fp16 x = screen_to_coord(
        Fl::event_x(), view->get_origin_x(), view->get_scaling());
      fp16 y = screen_to_coord(
        Fl::event_y(), view->get_origin_y(), view->get_scaling());
      u_fp32 min_dist = ~static_cast<u_fp32>(0);
      vector<Element *> *elements = view->get_figure()->get_elements();
      vector<Element *>::iterator i = elements->begin();
      Textual *closest = 0;
      while (i < elements->end()) {
        Element *elem = *i;
        Selectable *sel = dynamic_cast<Selectable *>(elem);
        Textual *tex = dynamic_cast<Textual *>(elem);
        if (sel && tex) {
          u_fp32 d = sel->select_distance_sqr(x, y);
          if (d < min_dist) {
            min_dist = d;
            closest = tex;
          }
        }
        i++;
      }
      if (closest) {
        int screen_dist_sqr = mul_fp32_fp32_int(
          min_dist, 
          mul_fp16_fp16_fp32(view->get_scaling(), view->get_scaling()));
        if (screen_dist_sqr <= VRF_DEFAULT_SELECT_DIST_SQR) {
          select(closest, view);
          return 1;
        }
      }

      // Otherwise create a new label
      Label *label = new Label(x, y, "");
      view->add_element(label);
      select(static_cast<Textual *>(label), view);
      
    } while (0);
    return 1;

  case FL_KEYBOARD:
    do {
      
      // Ignore events if no element selected
      if (!element)
        return 0;

      view->window()->make_current();

      // Check for known control keys
      switch (Fl::event_key()) {

      case FL_BackSpace:
        if (position > 0) {
          draw_xorred_text_cursor(view);
          draw_text(view, true);
          element->remove_text(position - 1, 1);
          draw_text(view);
          position--;
          draw_xorred_text_cursor(view);
        }
        return 1;

      case FL_Delete:
        if (position < element->get_text_length()) {
          draw_xorred_text_cursor(view);
          draw_text(view, true);
          element->remove_text(position, 1);
          draw_text(view);
          draw_xorred_text_cursor(view);
        }
        return 1;

      case FL_Left:
        if (position > 0) {
          draw_xorred_text_cursor(view);
          position--;
          draw_xorred_text_cursor(view);
        }
        return 1;

      case FL_Right:
        if (position < element->get_text_length()) {
          draw_xorred_text_cursor(view);
          position++;
          draw_xorred_text_cursor(view);
        }
        return 1;
      }

      // Check if text to insert
      if (Fl::event_length() > 0) {
        draw_xorred_text_cursor(view);
        draw_text(view, true);
        element->insert_text(position, Fl::event_text());
        draw_text(view);
        position += Fl::event_length();
        draw_xorred_text_cursor(view);
        return 1;
      }

    } while (0);
    return 1;

  case FL_FOCUS:
    return element ? 1 : 0;

  case FL_UNFOCUS:
    return 1;

  default:
    return 0;
  }
}

void LabelTool::select(Textual *elem, FigureView *view) {

  // Calculate the cursor position
  position = elem->get_text_length();
  element = elem;
  
  // Draw the cursor
  draw_xorred_text_cursor(view);

  // Request focus
  Fl::focus(view);
}

void LabelTool::unselect(FigureView *view) {
  if (element) {

    // Wipe the cursor
    draw_xorred_text_cursor(view);

    // Remove the element if it has become empty
    if (element->get_text_length() == 0) {
      Element *elem = dynamic_cast<Element *>(element);
      view->get_figure()->remove_element(elem);
    }
    
    element = 0;
  }
}

void LabelTool::update_position(int pos, FigureView *view) {

  // Wipe the old cursor
  draw_xorred_text_cursor(view);

  position = pos;

  // Draw the new cursor
  draw_xorred_text_cursor(view);
}

void LabelTool::draw_xorred_text_cursor(FigureView *view) {
  element->draw_text_cursor(position, 
                            view->get_origin_x(), view->get_origin_y(),
                            view->get_scaling(), true);
}

void LabelTool::draw_text(FigureView *view, bool xorred = false) {
  dynamic_cast<Element *>(element)->draw
    (view->get_origin_x(), view->get_origin_y(),
     view->get_scaling(), xorred);
}
