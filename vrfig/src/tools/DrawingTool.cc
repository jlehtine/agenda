// $Id: DrawingTool.cc,v 1.2 2001-05-23 12:47:51 jle Exp $

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

#if USE_EXPERIMENTAL_UI

#include <FL/Fl.H>
#include <FL/Enumerations.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Window.H>
#include "DrawingTool.hpp"
#include "Element.hpp"
#include "flext.hpp"
#include "icons/outline_icon.xbm"
#include "icons/filled_icon.xbm"

static Fl_Bitmap outline_bitmap
(outline_icon_bits, outline_icon_width, outline_icon_height);
static Fl_Bitmap filled_bitmap
(filled_icon_bits, filled_icon_width, filled_icon_height);

void DrawingTool::draw_current_line(FigureView *view) {
  fl_color(FL_WHITE);
  vector<int>::iterator i = points.begin();
  i += 2;
  while (i < points.end()) {
    if (i - points.begin() > 2)
      fle_xorred_point(*(i-2), *(i-1));
    fle_xorred_line(*(i-2), *(i-1), *i, *(i+1));
    i += 2;
  }
}

void DrawingTool::deactivated(FigureView *view) {
  if (drawing) {
    view->window()->make_current();
    draw_current_line(view);
  }
  points.clear();
  drawing = false;
}

void DrawingTool::draw(FigureView *view) {
  if (drawing) {
    draw_current_line(view);
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
      view->window()->make_current();
      fl_color(FL_WHITE);
      fle_xorred_point(Fl::event_x(), Fl::event_y());
    }
    break;

  case FL_RELEASE:
    if (drawing) {
      view->window()->make_current();
      draw_current_line(view);
      Element *element = Element::fit_element
        (&points, view->get_origin_x(), view->get_origin_y(),
         view->get_scaling());
      if (element != 0)
        view->add_element(element);
      drawing = false;
      points.clear();
    }
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
        fle_xorred_point(*(i-4), *(i-3));
        fle_xorred_line(*(i-4), *(i-3), *(i-2), *(i-1));
      }
    }
    break;

  default:
    return 0;

  }
  return 1;
}

#endif
