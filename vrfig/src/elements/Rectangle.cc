// $Id: Rectangle.cc,v 1.6 2001-05-24 18:47:09 jle Exp $

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

#include <FL/Enumerations.H>
#include <FL/fl_draw.H>
#include "Rectangle.hpp"
#include "flext.hpp"
#include "mathutil.hpp"
#include "vrfig.hpp"

/**
 * Information needed when deserializing XML data to rectangle.
 */
struct DeserializeInfo {
  void *old_data;
  ElementFactory *ef;
  XML_Parser parser;
  fp16 x, y;
  fp16 w, h;
  int depth;
};

/**
 * Element start handler for deserialization.
 */
static void start_handler(
  void *data, const XML_Char *name, const XML_Char **atts) {
  static const char *elem_position = VRF_DEFAULT_NAMESPACE "#position";
  static const char *elem_size = VRF_DEFAULT_NAMESPACE "#size";
  DeserializeInfo *info = reinterpret_cast<DeserializeInfo *>(data);

  if (info->depth == 0) {
    if (!strcmp(name, elem_position)) {
      while (*atts) {
        if (!strcmp(*atts, "x"))
          info->x = str_to_fp16(*(atts+1));
        else if (!strcmp(*atts, "y"))
          info->y = str_to_fp16(*(atts+1));
        atts += 2;
      }
    } else if (!strcmp(name, elem_size)) {
      while (*atts) {
        if (!strcmp(*atts, "width"))
          info->w = str_to_fp16(*(atts+1));
        else if (!strcmp(*atts, "height"))
          info->h = str_to_fp16(*(atts+1));
        atts += 2;
      }
    }
  }
  info->depth++;
}

/**
 * Element end handler for deserialization.
 */
static void end_handler(void *data, const XML_Char *name) {
  DeserializeInfo *info = reinterpret_cast<DeserializeInfo *>(data);
  if (info->depth == 0) {
    XML_SetElementHandler(info->parser, 0, 0);
    XML_SetUserData(info->parser, info->old_data);
    Rectangle *rect = new Rectangle(info->x, info->y, info->w, info->h);
    ElementFactory *ef = info->ef;
    delete info;
    ef->done(rect);
    return;
  }
  info->depth--;
}

const char *Rectangle::get_name_static() {
  static const char *name = "rectangle";
  return name;
}
  
const char *Rectangle::get_name() const {
  return get_name_static();
}

const char *Rectangle::get_namespace_static() {
  return vrf_default_namespace;
}
  
void Rectangle::get_bounding_box(fp16 &x, fp16 &y, fp16 &w, fp16 &h) const {
  x = Rectangle::x;
  y = Rectangle::y;
  w = Rectangle::w;
  h = Rectangle::h;
}
  
void Rectangle::draw(fp16 origin_x, fp16 origin_y, u_fp16 scaling,
                     bool xorred=false) const {
  int old_func;
  if (xorred) {
    fl_color(FL_WHITE);
    old_func = fle_xorred_mode();
  } else
    fl_color(FL_BLACK);
  fp16 sx = coord_to_screen(x, origin_x, scaling);
  fp16 sy = coord_to_screen(y, origin_y, scaling);
  fp16 sw = coord_to_screen(w, 0, scaling);
  fp16 sh = coord_to_screen(h, 0, scaling);
  fl_rect(sx, sy, sw, sh);
  if (xorred)
    fle_reset_mode(old_func);
}
  
ostream &Rectangle::serialize(ostream &os, const char *ns, int indent) const {
  static const char *elem_position = "position";
  static const char *elem_size = "size";

  output_indent(os, indent);
  output_ns_name(os << "<", ns, elem_position) <<
    " x=\"";
  write_fp16(os, x) << "\" y=\"";
  write_fp16(os, y) << "\"/>\n";
  output_indent(os, indent);
  output_ns_name(os << "<", ns, elem_size) <<
    " width=\"";
  write_fp16(os, w) << "\" height=\"";
  write_fp16(os, h) << "\"/>\n";
  return os;
}
 
void Rectangle::deserialize(XML_Parser *parser, ElementFactory *ef) {
  DeserializeInfo *info = new DeserializeInfo();
  info->old_data = XML_GetUserData(parser);
  info->ef = ef;
  info->parser = parser;
  info->depth = 0;
  XML_SetUserData(parser, info);
  XML_SetElementHandler(parser, start_handler, end_handler);
}

u_fp32 Rectangle::select_distance_sqr(fp16 x, fp16 y) const {
  u_fp32 min_dist = ~static_cast<u_fp32>(0);
  fp16 xs = controls[6];
  fp16 ys = controls[7];
  vector<fp16>::const_iterator i = controls.begin();
  while (i < controls.end()) {
    fp16 xn = *i;
    fp16 yn = *(i+1);
    u_fp32 dist = distance_to_line_sqr(x, y, xs, ys, xn - xs, yn - ys);
    if (dist < min_dist)
      min_dist = dist;
    xs = xn;
    ys = yn;
    i += 2;
  }
  return min_dist;
}
  
void Rectangle::move(fp16 xoff, fp16 yoff) {
  x += xoff;
  y += yoff;
  update_controls();
}
  
const vector<fp16> *Rectangle::get_control_points() const {
  return &controls;
}

void Rectangle::control(unsigned int i, fp16 _x, fp16 _y) {
  if (i >= 0 && i < 4) {
    switch (i) {
    case 0:
      w += x - _x;
      h += y - _y;
      x = _x;
      y = _y;
      if (w < 0) {
        x += w;
        w = 0;
      }
      if (h < 0) {
        y += h;
        h = 0;
      }
      break;
    case 1:
      w = _x - x;
      h += y - _y;
      y = _y;
      if (w < 0)
        w = 0;
      if (h < 0) {
        y += h;
        h = 0;
      }
      break;
    case 2:
      w = _x - x;
      h = _y - y;
      if (w < 0)
        w = 0;
      if (h < 0)
        h = 0;
      break;
    case 3:
      w += x - _x;
      x = _x;
      h = _y - y;
      if (w < 0) {
        x += w;
        w = 0;
      }
      if (h < 0) {
        h = 0;
      }
      break;
    }
    update_controls();
  }
}

void Rectangle::update_controls() {
  controls[0] = x;
  controls[1] = y;
  controls[2] = x + w - 1;
  controls[3] = y;
  controls[4] = x + w - 1;
  controls[5] = y + h - 1;
  controls[6] = x;
  controls[7] = y + h - 1;
}
