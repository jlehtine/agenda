// $Id: Ellipse.cc,v 1.3 2001-05-24 18:25:52 jle Exp $

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

#include <FL/fl_draw.H>
#include "Ellipse.hpp"
#include "flext.hpp"
#include "mathutil.hpp"
#include "vrfig.hpp"

/**
 * Information needed when deserializing XML data to ellipse.
 */
struct DeserializeInfo {
  void *old_data;
  ElementFactory *ef;
  XML_Parser parser;
  fp16 x, y;
  fp16 xr, yr;
  int depth;
};

/**
 * Element start handler for deserialization.
 */
static void start_handler(
  void *data, const XML_Char *name, const XML_Char **atts) {
  static const char *elem_position = VRF_DEFAULT_NAMESPACE "#position";
  static const char *elem_radius = VRF_DEFAULT_NAMESPACE "#radius";
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
    } else if (!strcmp(name, elem_radius)) {
      while (*atts) {
        if (!strcmp(*atts, "xr"))
          info->xr = str_to_fp16(*(atts+1));
        else if (!strcmp(*atts, "yr"))
          info->yr = str_to_fp16(*(atts+1));
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
    Ellipse *el = new Ellipse(
      info->x, info->y,
      info->x + info->xr, info->y + info->yr);
    info->ef->get_figure()->add_element(el);
    ElementFactory *ef = info->ef;
    delete info;
    ef->done();
    return;
  }
  info->depth--;
}

Ellipse::Ellipse(fp16 x, fp16 y, fp16 x1, fp16 y1): controls(4) {
  controls[0] = x;
  controls[1] = y;
  controls[2] = x1;
  controls[3] = y1;
}

const char *Ellipse::get_name_static() {
  static const char *name = "ellipse";
  return name;
}

const char *Ellipse::get_name() const {
  return get_name_static();
}
  
const char *Ellipse::get_namespace_static() {
  return vrf_default_namespace;
}

void Ellipse::get_bounding_box(fp16 &x, fp16 &y, fp16 &w, fp16 &h) const {
  u_fp16 hw = abs(controls[0] - controls[2]);
  u_fp16 hh = abs(controls[1] - controls[3]);
  x = controls[0] - hw;
  y = controls[1] - hh;
  w = (hw << 1) + 1;
  h = (hh << 1) + 1;
}
  
void Ellipse::draw(fp16 origin_x, fp16 origin_y, u_fp16 scaling,
                   bool xorred=false) const {
  fp16 x, y, w, h;
  get_bounding_box(x, y, w, h);
  int old_func;
  if (xorred) {
    fl_color(FL_WHITE);
    old_func = fle_xorred_mode();
  } else
    fl_color(FL_BLACK);
  fl_arc(coord_to_screen(x, origin_x, scaling),
         coord_to_screen(y, origin_y, scaling),
         coord_to_screen(w, 0, scaling),
         coord_to_screen(h, 0, scaling),
         0, 360);
  if (xorred)
    fle_reset_mode(old_func);
}
  
ostream &Ellipse::serialize(ostream &os, const char *ns, int indent) const {
  static const char *elem_position = "position";
  static const char *elem_radius = "radius";

  output_indent(os, indent);
  output_ns_name(os << "<", ns, elem_position) <<
    " x=\"";
  write_fp16(os, controls[0]) << "\" y=\"";
  write_fp16(os, controls[1]) << "\"/>\n";
  output_indent(os, indent);
  output_ns_name(os << "<", ns, elem_radius) <<
    " xr=\"";
  write_fp16(os, abs(controls[0] - controls[2])) << "\" yr=\"";
  write_fp16(os, abs(controls[1] - controls[3])) << "\"/>\n";
  return os;
}
  
void Ellipse::deserialize(XML_Parser *parser, ElementFactory *ef) {
  DeserializeInfo *info = new DeserializeInfo();
  info->old_data = XML_GetUserData(parser);
  info->ef = ef;
  info->parser = parser;
  info->depth = 0;
  XML_SetUserData(parser, info);
  XML_SetElementHandler(parser, start_handler, end_handler);
}

u_fp32 Ellipse::select_distance_sqr(fp16 x, fp16 y) const {

  fp16 hw = abs(controls[0] - controls[2]);
  fp16 hh = abs(controls[1] - controls[3]);

  // Check if actually a point
  if (hw == 0 && hh == 0)
    return vector_length_sqr_fp16_fp32(x - controls[0], y - controls[1]);

  // Transform the coordinates to make comparison in circle space
  fp16 rad;
  fp16 cx, cy;
  if (hw > hh) {
    u_fp32 m = div_fp16(hh, hw);
    rad = hh;
    cx = mul_fp16(m, x - controls[0]);
    cy = y - controls[1];
  } else {
    fp16 m = div_fp16(hw, hh);
    rad = hw;
    cx = x - controls[0];
    cy = mul_fp16(m, y - controls[1]);
  }

  // Return comparison result
  fp32 dist = vector_length_sqr_fp16_fp32(cx, cy)
    - mul_fp16_fp16_fp32(rad, rad);
  return dist > 0 ? dist : -dist;
}
  
void Ellipse::move(fp16 xoff, fp16 yoff) {
  controls[0] += xoff;
  controls[1] += yoff;
  controls[2] += xoff;
  controls[3] += yoff;
}
  
const vector<fp16> *Ellipse::get_control_points() const {
  return &controls;
}

void Ellipse::control(unsigned int i, fp16 x, fp16 y) {
  if (i == 0) {
    controls[2] += x - controls[0];
    controls[3] += y - controls[1];
    controls[0] = x;
    controls[1] = y;
  } else if (i == 1) {
    controls[2] = x;
    controls[3] = y;
  }
}
