// $Id: PolyLineTool.cc,v 1.3 2001-05-23 12:47:52 jle Exp $

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
#include "PolyLineTool.hpp"
#include "icons/polyline_icon.xbm"
#include "elements/PolyLine.hpp"
#include "flext.hpp"

#define CONTINUE_DIST_SQR 16

static Fl_Bitmap polyline_bitmap
(polyline_icon_bits, polyline_icon_width, polyline_icon_height);

const char *PolyLineTool::get_name() const {
  static const char *name = "polyline";
  return name;
}

void PolyLineTool::draw_icon(int x, int y, int w, int h) const {
  polyline_bitmap.draw(x, y, w, h,
                       (polyline_bitmap.w - w) >> 1,
                       (polyline_bitmap.h - h) >> 1);
}

void PolyLineTool::deactivated(FigureView *view) {
  last_polyline = 0;
  drawing_segment = false;
}

void PolyLineTool::draw(FigureView *view) {
  fl_color(FL_WHITE);
  if (drawing_segment)
    fle_xorred_line(last_x, last_y, seg_x, seg_y);
  else if (last_polyline) {
    int old_func = fle_xorred_mode();
    Selectable::draw_select_helper(last_x, last_y);
    fle_reset_mode(old_func);
  }
}

int PolyLineTool::handle(int event, FigureView *view) {
  view->window()->make_current();

  switch (event) {
    vector<fp16> *vertices;
    vector<fp16>::const_iterator start;
    int old_func;
    int start_x, start_y;

  case FL_PUSH:
    if (drawing_segment)
      return 0;

    // Remove the old select helper
    if (last_polyline) {
      fl_color(FL_WHITE);
      old_func = fle_xorred_mode();
      Selectable::draw_select_helper(last_x, last_y);
      fle_reset_mode(old_func);
    }

    // Check if a new polyline should be created
    if (!last_polyline 
        || vector_length_sqr_int(Fl::event_x() - last_x,
                                 Fl::event_y() - last_y) > CONTINUE_DIST_SQR) {
      PolyLine *pl = new PolyLine();
      vertices = pl->get_vertices();
      last_x = Fl::event_x();
      last_y = Fl::event_y();
      vertices->insert(vertices->end(),
                       screen_to_coord(last_x, view->get_origin_x(),
                                       view->get_scaling()));
      vertices->insert(vertices->end(),
                       screen_to_coord(last_y, view->get_origin_y(),
                                       view->get_scaling()));
      view->add_element(pl);
      last_polyline = pl;
    }

    drawing_segment = true;
    seg_x = Fl::event_x();
    seg_y = Fl::event_y();
    fl_color(FL_WHITE);
    fle_xorred_line(last_x, last_y, seg_x, seg_y);
    return 1;

  case FL_DRAG:
    if (!drawing_segment)
      return 0;
    fl_color(FL_WHITE);
    fle_xorred_line(last_x, last_y, seg_x, seg_y);
    seg_x = Fl::event_x();
    seg_y = Fl::event_y();
    fle_xorred_line(last_x, last_y, seg_x, seg_y);
    return 1;

  case FL_RELEASE:
    if (!drawing_segment)
      return 0;
    fl_color(FL_WHITE);
    fle_xorred_line(last_x, last_y, seg_x, seg_y);
    seg_x = Fl::event_x();
    seg_y = Fl::event_y();
    vertices = last_polyline->get_vertices();
    start = vertices->begin();
    start_x = coord_to_screen(*(start++), view->get_origin_x(),
                              view->get_scaling());
    start_y = coord_to_screen(*start, view->get_origin_x(),
                              view->get_scaling());
    fl_color(FL_BLACK);
    if (vector_length_sqr_int(start_x - seg_x, start_y - seg_y)
        <= CONTINUE_DIST_SQR) {
      fl_line(last_x, last_y, start_x, start_y);
      last_polyline->set_closed(true);
      last_polyline = 0;
    } else {
      fl_line(last_x, last_y, seg_x, seg_y);
      vertices->insert(vertices->end(),
                       screen_to_coord(seg_x, view->get_origin_x(),
                                       view->get_scaling()));
      vertices->insert(vertices->end(),
                       screen_to_coord(seg_y, view->get_origin_y(),
                                       view->get_scaling()));
      last_x = seg_x;
      last_y = seg_y;
      fl_color(FL_WHITE);
      old_func = fle_xorred_mode();
      Selectable::draw_select_helper(last_x, last_y);
      fle_reset_mode(old_func);
    }
    drawing_segment = false;
    return 1;

  default:
    return 0;
  }
}
