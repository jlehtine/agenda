// $Id: PolyLineTool.cc,v 1.5 2001-05-27 17:54:36 jle Exp $

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

/**
 * Finds the polyline end closest to the specified point. Points more than
 * the maximum connect distance are ignored.
 *
 * @param view the current view
 * @param x the x coordinate
 * @param y the y coordinate
 * @param first_vertex return value: whether the closest point is the first \
 *    vertex (or the last one)
 * @return the closes polyline or null if none
 */
PolyLine *find_connect_point(FigureView *view, fp16 x, fp16 y, 
                             bool &first_vertex);

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
  polyline = 0;
}

void PolyLineTool::draw(FigureView *view) {
  fl_color(FL_WHITE);

  // Draw select helpers
  int old_func = fle_xorred_mode();
  const vector<Element *> *elements = view->get_figure()->get_elements();
  vector<Element *>::const_iterator i = elements->begin();
  while (i < elements->end()) {
    PolyLine *pl = dynamic_cast<PolyLine *>(*i);
    if (pl && !(pl->is_closed())) {
      const vector<fp16> *vertices = pl->get_vertices();
      Selectable::draw_select_helper
        ((*vertices)[0], (*vertices)[1],
         view->get_origin_x(), view->get_origin_y(), view->get_scaling());
      int s = vertices->size();
      if (s > 2)
        Selectable::draw_select_helper
          ((*vertices)[s-2], (*vertices)[s-1],
           view->get_origin_x(), view->get_origin_y(), view->get_scaling());
    }
    i++;
  }

  // Draw the line being drawn and remove the old selector
  if (polyline) {
    fl_line(start_x, start_y, last_x, last_y);
    const vector<fp16> *vertices = polyline->get_vertices();
    int s = vertices->size();
    Selectable::draw_select_helper
      ((*vertices)[extend_first ? 0 : s - 2],
       (*vertices)[extend_first ? 1 : s - 1],
       view->get_origin_x(), view->get_origin_y(), view->get_scaling());
  }
  fle_reset_mode(old_func);
}

int PolyLineTool::handle(int event, FigureView *view) {
  view->window()->make_current();

  switch (event) {

  case FL_PUSH:
    if (polyline)
      return 0;
    do {
      
      // Find the closest possible starting point
      fp16 fx = screen_to_coord(Fl::event_x(), 
                                view->get_origin_x(), view->get_scaling());
      fp16 fy = screen_to_coord(Fl::event_y(),
                                view->get_origin_y(), view->get_scaling());
      polyline = find_connect_point(view, fx, fy, extend_first);
      
      // Check if a new polyline should be created
      fl_color(FL_WHITE);
      if (!polyline) {
        start_x = Fl::event_x();
        start_y = Fl::event_y();
        PolyLine *pl = new PolyLine();
        vector<fp16> *vertices = pl->get_vertices();
        vertices->insert(vertices->end(),
                         screen_to_coord(start_x, view->get_origin_x(),
                                         view->get_scaling()));
        vertices->insert(vertices->end(),
                         screen_to_coord(start_y, view->get_origin_y(),
                                         view->get_scaling()));
        view->get_figure()->add_element(pl);
        polyline = pl;
        extend_first = false;
        new_polyline = true;
      } else {

        // Adjust start coordinates
        vector<fp16> *vertices = polyline->get_vertices();
        int s = vertices->size();
        start_x = coord_to_screen
          ((*vertices)[extend_first ? 0 : s - 2], 
           view->get_origin_x(), view->get_scaling());
        start_y = coord_to_screen
          ((*vertices)[extend_first ? 1 : s - 1], 
           view->get_origin_y(), view->get_scaling());

        // Hide the extended select helper
        fl_color(FL_WHITE);
        int old_func = fle_xorred_mode();
        Selectable::draw_select_helper(start_x, start_y);
        fle_reset_mode(old_func);
        new_polyline = false;
      }

      last_x = Fl::event_x();
      last_y = Fl::event_y();
      fle_xorred_line(start_x, start_y, last_x, last_y);
    } while (0);
    return 1;

  case FL_DRAG:
    if (!polyline)
      return 0;
    fl_color(FL_WHITE);
    fle_xorred_line(start_x, start_y, last_x, last_y);
    last_x = Fl::event_x();
    last_y = Fl::event_y();
    fle_xorred_line(start_x, start_y, last_x, last_y);
    return 1;

  case FL_RELEASE:
    if (!polyline)
      return 0;
    do {
      fl_color(FL_BLACK);
      fl_line(start_x, start_y, last_x, last_y);
      vector<fp16> *vertices = polyline->get_vertices();
      vertices->insert(extend_first ? vertices->begin() : vertices->end(),
                       screen_to_coord(last_x, view->get_origin_x(),
                                       view->get_scaling()));
      vertices->insert(extend_first ? vertices->begin() + 1 : vertices->end(),
                       screen_to_coord(last_y, view->get_origin_y(),
                                       view->get_scaling()));

      // Draw new select helpers
      fl_color(FL_WHITE);
      int old_func = fle_xorred_mode();
      if (new_polyline)
        Selectable::draw_select_helper
          ((*vertices)[0], (*vertices)[1],
           view->get_origin_x(), view->get_origin_y(), view->get_scaling());
      int s = vertices->size();
      Selectable::draw_select_helper
        ((*vertices)[extend_first ? 0 : s - 2],
         (*vertices)[extend_first ? 1 : s - 1],
         view->get_origin_x(), view->get_origin_y(), view->get_scaling());
      fle_reset_mode(old_func);
    } while (0);
    polyline = 0;
    return 1;

  default:
    return 0;
  }
}

PolyLine *find_connect_point(FigureView *view, fp16 x, fp16 y, 
                             bool &first_vertex) {
  PolyLine *polyline = 0;
  u_fp32 min_dist = ~static_cast<u_fp32>(0);
  const vector<Element *> *elements = view->get_figure()->get_elements();
  vector<Element *>::const_iterator i = elements->begin();
  while (i < elements->end()) {
    PolyLine *pl = dynamic_cast<PolyLine *>(*i);
    if (pl) {
      const vector<fp16> *vertices = pl->get_vertices();
      int s = vertices->size();
      u_fp32 dist;
      if (s > 2) {
        dist = vector_length_sqr_fp16_fp32
          (x - (*vertices)[0], y - (*vertices)[1]);
        if (dist < min_dist) {
          polyline = pl;
          first_vertex = true;
          min_dist = dist;
        }
      }
      dist = vector_length_sqr_fp16_fp32
        (x - (*vertices)[s-2], y - (*vertices)[s-1]);
      if (dist < min_dist) {
        polyline = pl;
        first_vertex = false;
        min_dist = dist;
      }
    }
    i++;
  }
  if (coord_to_screen(fp32_to_fp16(min_dist), view->get_scaling())
      > CONTINUE_DIST_SQR)
    polyline = 0;
  return polyline;
}
