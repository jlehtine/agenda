// $Id: PolyLine.cc,v 1.14 2001-06-10 19:13:10 jle Exp $

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
#include <stdio.h>
#include <string.h>
#include <FL/fl_draw.H>
#include <FL/Enumerations.H>
#include "PolyLine.hpp"
#include "mathutil.hpp"
#include "flext.hpp"
#include "ElementFactory.hpp"

#define CLOSED_MAX_GAP 16
#define DEVIATION_MAX 25

/**
 * Information needed when deserializing XML data to polyline.
 */
struct DeserializeInfo {
  void *old_data;
  ElementFactory *ef;
  XML_Parser parser;
  PolyLine *pl;
  int depth;
  bool parsing_points;
};

/**
 * Element start handler for deserialization.
 */
static void start_handler(
  void *data, const XML_Char *name, const XML_Char **atts) {
  static const char *points_name = VRF_DEFAULT_NAMESPACE "#points";
  static const char *point_name = VRF_DEFAULT_NAMESPACE "#point";
  DeserializeInfo *info = reinterpret_cast<DeserializeInfo *>(data);
  
  // Check if points section found
  if (info->depth == 0) {
    if (!strcmp(name, points_name)) {
      info->parsing_points = true;
      while (*atts) {
        if (!strcmp(*atts, "closed") && !strcmp(*(atts+1), "true"))
          info->pl->set_closed(true);
        atts += 2;
      }
    }
  }

  // Check if a new point found
  else if (info->depth == 1 && info->parsing_points) {
    if (!strcmp(name, point_name)) {
      fp16 x=0, y=0;
      while (*atts) {
        if (!strcmp(*atts, "x"))
          x = str_to_fp16(*(atts+1));
        else if (!strcmp(*atts, "y"))
          y = str_to_fp16(*(atts+1));
        atts += 2;
      }
      vector<Point> *vertices = info->pl->get_vertices();
      vertices->insert(vertices->end(), Point(x, y));
    }
  }
  
  info->depth++;
}

/**
 * Element end handler for deserialization.
 */
static void end_handler(void *data, const XML_Char *name) {
  DeserializeInfo *info = reinterpret_cast<DeserializeInfo *>(data);
  if (info->depth == 1)
    info->parsing_points = false;
  else if (info->depth == 0) {
    XML_SetElementHandler(info->parser, 0, 0);
    XML_SetUserData(info->parser, info->old_data);
    Element *pl = info->pl;
    ElementFactory *ef = info->ef;
    delete info;
    ef->done(pl);
    return;
  }
  info->depth--;
}

PolyLine::~PolyLine() {
  points.clear();
}


const char *PolyLine::get_name_static() {
  static const char *name = "polyline";
  return name;
}

const char *PolyLine::get_name() const {
  return get_name_static();
}

const char *PolyLine::get_namespace_static() {
  return vrf_default_namespace;
}

void PolyLine::draw(fp16 origin_x, fp16 origin_y, u_fp16 scaling,
                    bool xorred) const {

  // Find the starting point for the polyline
  vector<Point>::const_iterator i = points.begin();
  int last_x, last_y;
  if (closed) {
    vector<Point>::const_iterator last = points.end();
    last--;
    (*last).to_screen(origin_x, origin_y, scaling, last_x, last_y);
  } else {
    (*i).to_screen(origin_x, origin_y, scaling, last_x, last_y);
    i++;
  }

  // Prepare for drawing
  int old_func = 0;
  if (xorred) {
    old_func = fle_xorred_mode();
    fl_color(FL_WHITE);
    fl_point(last_x, last_y);
  } else
    fl_color(FL_BLACK);

  // Draw the polyline
  while (i < points.end()) {
    int x, y;
    (*i).to_screen(origin_x, origin_y, scaling, x, y);
    fl_line(last_x, last_y, x, y);
    last_x = x;
    last_y = y;
    i++;
  }

  // Restore drawing mode
  if (xorred)
    fle_reset_mode(old_func);
}

void PolyLine::get_bounding_box(fp16 &x, fp16 &y, fp16 &w, fp16 &h) const {
  vector<Point>::const_iterator i = points.begin();
  x = (*i).x;
  y = (*i).y;
  w = 1;
  h = 1;
  while (i < points.end()) {
    int px = (*i).x;
    if (px < x) {
      w += x - px;
      x = px;
    } else if (px >= x + w)
      w += px - (x + w) + 1;
    int py = (*i).y;
    if (py < y) {
      h += y - py;
      y = py;
    } else if (py >= y + h)
      h += py - (y + h) + 1;
    i++;
  }
}

ostream &PolyLine::serialize(ostream &os, const char *ns, int indent) const {
  static const char *elem_points = "points";
  static const char *elem_point = "point";

  output_indent(os, indent);
  output_ns_name(os << "<", ns, elem_points) << 
    " num=\"" << points.size() << 
    "\" closed=\"" << (closed ? "true" : "false") << "\">\n";
  vector<Point>::const_iterator i = points.begin();
  while (i < points.end()) {
    output_indent(os, indent+2);
    output_ns_name(os << "<", ns, elem_point) <<
      " x=\"";
    write_fp16(os, (*i).x) << "\" y=\"";
    write_fp16(os, (*i).y) << "\"/>\n";
    i++;
  }
  output_indent(os, indent);
  output_ns_name(os << "</", ns, elem_points) << ">\n";
  return os;
}

void PolyLine::deserialize(XML_Parser *parser, ElementFactory *ef) {
  DeserializeInfo *info = new DeserializeInfo();
  info->old_data = XML_GetUserData(parser);
  info->ef = ef;
  info->parser = parser;
  info->pl = new PolyLine();
  info->depth = 0;
  info->parsing_points = false;
  XML_SetUserData(parser, info);
  XML_SetElementHandler(parser, start_handler, end_handler);
}

#if 0
void PolyLine::draw_select_helpers(
  int origin_x, int origin_y, unsigned int scaling, bool xorred=false) const {
  int old_func;
  if (xorred) {
    old_func = fle_xorred_mode();
    fl_color(FL_WHITE);
  } else
    fl_color(FL_BLACK);
  vector<fp16>::const_iterator i = points.begin();
  while (i < points.end()) {
    draw_select_helper((*i).x, (*i).y, origin_x, origin_y, scaling);
    i++;
  }
  fle_reset_mode(old_func);
}
#endif

u_fp32 PolyLine::select_distance_sqr(fp16 x, fp16 y) const {

  // Find the starting point for checks
  fp16 last_x, last_y;
  vector<Point>::const_iterator i = points.begin();
  if (closed) {
    vector<Point>::const_iterator last = points.end();
    last--;
    last_x = (*last).x;
    last_y = (*last).y;
  } else {
    last_x = (*i).x;
    last_y = (*i).y;
    i++;
  }

  // Find the minimum distance
  u_fp32 min_dist = ~static_cast<u_fp32>(0);
  while (i < points.end()) {
    u_fp32 dist = distance_to_line_sqr(
      x, y, last_x, last_y, (*i).x - last_x, (*i).y - last_y);
    if (dist < min_dist)
      min_dist = dist;
    last_x = (*i).x;
    last_y = (*i).y;
    i++;
  }
  return min_dist;
}

void PolyLine::move(fp16 xoff, fp16 yoff) {
  vector<Point>::iterator i = points.begin();
  while (i < points.end()) {
    (*i).x += xoff;
    (*i).y += yoff;
    i++;
  }
}

const vector<Point> *PolyLine::get_control_points() const {
  return &points;
}
  
void PolyLine::control(unsigned int i, fp16 x, fp16 y) {
  if (i >= 0 && i < points.size()) {
    points[i].x = x;
    points[i].y = y;
  }
}
