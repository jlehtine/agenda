// $Id: PolyLineTool.cc,v 1.8 2001-05-29 18:05:11 jle Exp $

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

#include <algo.h>
#include <iterator.h>
#include <FL/Fl.H>
#include <FL/Enumerations.H>
#include <FL/Fl_Bitmap.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include "PolyLineTool.hpp"
#include "Action.hpp"
#include "icons/polyline_icon.xbm"
#include "elements/PolyLine.hpp"
#include "flext.hpp"

#define CONTINUE_DIST_SQR 16

static Fl_Bitmap polyline_bitmap
(polyline_icon_bits, polyline_icon_width, polyline_icon_height);

/**
 * An action for undoing polyline operations.
 */
class PolyLineAction : public Action {

protected:

  /** The view used */
  FigureView *view;

  /** The primary polyline being modified */
  PolyLine *polyline;

  /** The type of action performed */
  enum { NEW_VERTEX, CLOSED, CONNECT } type;

  /** Whether the first or last vertex was extended (for NEW_VERTEX) */
  bool extend_first;

  /** Whether a new polyline was created (for NEW_VERTEX) */
  bool new_polyline;

  /** The old vertices of the primary polyline (for CONNECT) */
  vector<Point> old_vertices;

  /** The other polyline (for CONNECT) */
  PolyLine *sec_polyline;

public:

  inline PolyLineAction(FigureView *view, PolyLine *polyline, 
                        bool extend_first, bool new_polyline):
    view(view), polyline(polyline), type(NEW_VERTEX), 
    extend_first(extend_first), new_polyline(new_polyline), old_vertices() {}
    
  inline PolyLineAction(FigureView *view, PolyLine *polyline):
    view(view), polyline(polyline), type(CLOSED), old_vertices() {}

  inline PolyLineAction(FigureView *view, PolyLine *polyline,
                        vector<Point> *_old_vertices, PolyLine *sec_polyline):
    view(view), polyline(polyline), type(CONNECT), old_vertices(), 
    sec_polyline(sec_polyline) {
    old_vertices.resize(_old_vertices->size());
    copy(_old_vertices->begin(), _old_vertices->end(), old_vertices.begin());
  }

  virtual void undo() {
    switch (type) {

    case NEW_VERTEX:
      do {
        if (new_polyline) {
          view->get_figure()->remove_element(polyline);
          delete polyline;
        } else {
          vector<Point> *vertices = polyline->get_vertices();
          if (extend_first)
            vertices->erase(vertices->begin());
          else {
            vector<Point>::iterator i = vertices->end();
            vertices->erase(--i);
          }
        }
      } while (0);
      break;
    
    case CLOSED:
      polyline->set_closed(false);
      break;

    case CONNECT:
      do {
        vector<Point> *vertices = polyline->get_vertices();
        vertices->resize(old_vertices.size());
        copy(old_vertices.begin(), old_vertices.end(), vertices->begin());
        view->get_figure()->add_element(sec_polyline);
      } while (0);
      break;
    }
    view->redraw();
  }

  virtual void commit() {
    if (type == CONNECT)
      delete sec_polyline;
  }
};

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
      vector<Point> *vertices = pl->get_vertices();
      Point first_vertex = *(vertices->begin());
      Selectable::draw_select_helper
        (first_vertex.x, first_vertex.y,
         view->get_origin_x(), view->get_origin_y(), view->get_scaling());
      if (vertices->size() > 1) {
        vector<Point>::const_iterator ii = vertices->end();
        ii--;
        Selectable::draw_select_helper
          ((*ii).x, (*ii).y,
           view->get_origin_x(), view->get_origin_y(), view->get_scaling());
      }
    }
    i++;
  }

  // Draw the line being drawn and remove the old selector
  if (polyline) {
    fl_line(start_x, start_y, last_x, last_y);
    vector<Point>::const_iterator i;
    if (extend_first)
      i = polyline->get_vertices()->begin();
    else {
      i = polyline->get_vertices()->end();
      i--;
    }
    Selectable::draw_select_helper
      ((*i).x, (*i).y,
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
        vector<Point> *vertices = pl->get_vertices();
        vertices->insert(vertices->end(), Point(fx, fy));
        view->get_figure()->add_element(pl);
        polyline = pl;
        extend_first = false;
        new_polyline = true;
      } else {

        // Adjust start coordinates
        vector<Point>::const_iterator i;
        if (extend_first)
          i = polyline->get_vertices()->begin();
        else {
          i = polyline->get_vertices()->end();
          i--;
        }
        start_x = coord_to_screen
          ((*i).x, view->get_origin_x(), view->get_scaling());
        start_y = coord_to_screen
          ((*i).y, view->get_origin_y(), view->get_scaling());

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
      fl_color(FL_WHITE);
      fle_xorred_line(start_x, start_y, last_x, last_y);
      fp16 fx = screen_to_coord(last_x, 
                                view->get_origin_x(), view->get_scaling());
      fp16 fy = screen_to_coord(last_y,
                                view->get_origin_y(), view->get_scaling());
      
      // Check if closed polygon
      bool fv;
      PolyLine *pl = find_connect_point(view, fx, fy, fv);
      if (pl == polyline && fv != extend_first && !new_polyline) {
        polyline->set_closed(true);
        vector<Point>::const_iterator i;
        if (fv)
          i = polyline->get_vertices()->begin();
        else {
          i = polyline->get_vertices()->end();
          i--;
        }
        int ox = coord_to_screen(
          (*i).x, view->get_origin_x(), view->get_scaling());
        int oy = coord_to_screen(
          (*i).y, view->get_origin_x(), view->get_scaling());
        int old_func = fle_xorred_mode();
        Selectable::draw_select_helper(ox, oy);
        fle_reset_mode(old_func);
        fl_color(FL_BLACK);
        fl_line(start_x, start_y, ox, oy);

        view->get_action_buffer()->add_action(
          new PolyLineAction(view, polyline));
      }

      // Check if connecting two polylines
      else if (pl && pl != polyline) {

        // Update view
        const vector<Point> *vertices2 = pl->get_vertices();
        vector<Point>::const_iterator i2;
        if (fv)
          i2 = vertices2->begin();
        else {
          i2 = vertices2->end();
          i2--;
        }
        last_x = coord_to_screen
          ((*i2).x, view->get_origin_x(), view->get_scaling());
        last_y = coord_to_screen
          ((*i2).y, view->get_origin_x(), view->get_scaling());
        int old_func = fle_xorred_mode();
        Selectable::draw_select_helper(last_x, last_y);
        fle_reset_mode(old_func);
        fl_color(FL_BLACK);
        fl_line(start_x, start_y, last_x, last_y);

        vector<Point> *vertices1 = polyline->get_vertices();
        view->get_action_buffer()->add_action(
          new PolyLineAction(view, polyline, vertices1, pl));

        // Combine the polylines
        if (extend_first)
          reverse(vertices1->begin(), vertices1->end());
        back_insert_iterator<vector<Point> > bii(*vertices1);
        if (fv)
          copy(vertices2->begin(), vertices2->end(), bii);
        else 
          copy(vertices2->rbegin(), vertices2->rend(), bii);
        
        // Remove the unnecessary polyline
        view->get_figure()->remove_element(pl);
      }

      // Create new vertex
      else {
        fl_color(FL_BLACK);
        fl_line(start_x, start_y, last_x, last_y);
        vector<Point> *vertices = polyline->get_vertices();
        vertices->insert(extend_first ? vertices->begin() : vertices->end(),
                         Point(fx, fy));
        
        // Draw new select helpers
        fl_color(FL_WHITE);
        int old_func = fle_xorred_mode();
        if (new_polyline)
          Selectable::draw_select_helper
            ((*vertices)[0].x, (*vertices)[0].y,
             view->get_origin_x(), view->get_origin_y(), view->get_scaling());
        Selectable::draw_select_helper(last_x, last_y);
        fle_reset_mode(old_func);

        view->get_action_buffer()->add_action(
          new PolyLineAction(view, polyline, extend_first, new_polyline));
      }
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
      const vector<Point> *vertices = pl->get_vertices();
      int s = vertices->size();
      u_fp32 dist;
      if (s > 1) {
        dist = vector_length_sqr_fp16_fp32
          (x - (*vertices)[0].x, y - (*vertices)[0].y);
        if (dist < min_dist) {
          polyline = pl;
          first_vertex = true;
          min_dist = dist;
        }
      }
      dist = vector_length_sqr_fp16_fp32
        (x - (*vertices)[s-1].x, y - (*vertices)[s-1].y);
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
