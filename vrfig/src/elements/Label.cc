// $Id: Label.cc,v 1.1 2001-05-25 22:31:01 jle Exp $

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

#include <stdlib.h>
#include <string.h>
#include <FL/Enumerations.H>
#include <FL/fl_draw.H>
#include "Label.hpp"
#include "mathutil.hpp"
#include "flext.hpp"
#include "vrfig.hpp"

/**
 * Information needed when deserializing XML data to rectangle.
 */
struct DeserializeInfo {
  void *old_data;
  ElementFactory *ef;
  XML_Parser parser;
  fp16 x, y;
  char *text;
  int text_len;
  bool collect_text;
  int depth;
};

static void text_handler(void *data, const XML_Char *s, int len);

/**
 * Element start handler for deserialization.
 */
static void start_handler(
  void *data, const XML_Char *name, const XML_Char **atts) {
  static const char *elem_position = VRF_DEFAULT_NAMESPACE "#position";
  static const char *elem_text = VRF_DEFAULT_NAMESPACE "#text";
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
    } else if (!strcmp(name, elem_text)) {
      XML_SetCharacterDataHandler(info->parser, text_handler);
      info->collect_text = true;
      info->text = 0;
      info->text_len = 0;
    }
  }
  info->depth++;
}

/**
 * Element end handler for deserialization.
 */
static void end_handler(void *data, const XML_Char *name) {
  DeserializeInfo *info = reinterpret_cast<DeserializeInfo *>(data);
  if (info->collect_text) {
    XML_SetCharacterDataHandler(info->parser, 0);
    info->collect_text = false;
  } else if (info->depth == 0) {
    XML_SetElementHandler(info->parser, 0, 0);
    XML_SetUserData(info->parser, info->old_data);
    ElementFactory *ef = info->ef;
    if (info->text_len > 0) {
      Label *label = new Label(info->x, info->y, info->text);
      free(info->text);
      delete info;
      ef->done(label);
    } else {
      delete info;
      ef->done(0);
    }
    return;
  }
  info->depth--;
}

/**
 * Character data handler for deserialization.
 */
static void text_handler(void *data, const XML_Char *s, int len) {
  DeserializeInfo *info = reinterpret_cast<DeserializeInfo *>(data);
  info->text = static_cast<char *>(
    realloc(info->text, info->text_len + len + 1));
  if (!(info->text)) {
    XML_SetElementHandler(info->parser, 0, 0);
    XML_SetCharacterDataHandler(info->parser, 0);
    ElementFactory *ef = info->ef;
    delete info;
    ef->msg_error("Memory allocation failed.");
  }
  memcpy(info->text + info->text_len, s, len);
  info->text_len += len;
  info->text[info->text_len] = '\0';
}

const char *Label::get_name_static() {
  static const char *name = "label";
  return name;
}

const char *Label::get_name() const {
  return get_name_static();
}
  
const char *Label::get_namespace_static() {
  return vrf_default_namespace;
}
  
void Label::get_bounding_box(fp16 &_x, fp16 &_y, fp16 &_w, fp16 &_h) const {
  fl_font(FL_HELVETICA + FL_BOLD, 16);
  int w=1000, h;
  fl_measure(text.c_str(), w, h);
  _x = x;
  _y = y;
  _w = int_to_fp16(w);
  _h = int_to_fp16(h);
}
  
void Label::draw(fp16 origin_x, fp16 origin_y, u_fp16 scaling,
                 bool xorred=false) const {
  int old_func;
  if (xorred) {
    fl_color(FL_WHITE);
    old_func = fle_xorred_mode();
  } else
    fl_color(FL_BLACK);
  
  // Check whether to draw the text or placeholder box
  int font_size = coord_to_screen(int_to_fp16(16), 0, scaling);
  if (font_size < 8 || font_size > 32) {
    fp16 bx, by, bw, bh;
    get_bounding_box(bx, by, bw, bh);
    fl_rectf(coord_to_screen(bx, origin_x, scaling),
             coord_to_screen(by, origin_y, scaling),
             coord_to_screen(bw, 0, scaling),
             coord_to_screen(bh, 0, scaling));
  } else {

    // Make it either 8, 16 or 32
    if (font_size <= 16) {
      if (abs(font_size - 16) <= abs(font_size - 8))
        font_size = 16;
      else
        font_size = 8;
    } else {
      if (abs(font_size - 16) <= abs(font_size - 32))
        font_size = 16;
      else
        font_size = 32;
    }

    fl_font(FL_HELVETICA + FL_BOLD, font_size);
    fl_draw(text.c_str(),
            coord_to_screen(x, origin_x, scaling),
            coord_to_screen(y, origin_y, scaling),
            1000, 1000, FL_ALIGN_TOP_LEFT);
  }
  if (xorred)
    fle_reset_mode(old_func);
}
  
ostream &Label::serialize(ostream &os, const char *ns, int indent) const {
  static const char *elem_position = "position";
  static const char *elem_text = "text";

  output_indent(os, indent);
  output_ns_name(os << "<", ns, elem_position) <<
    " x=\"";
  write_fp16(os, x) << "\" y=\"";
  write_fp16(os, y) << "\"/>\n";
  output_indent(os, indent);
  output_ns_name(os << "<", ns, elem_text) << ">" << text;
  output_ns_name(os << "</", ns, elem_text) << ">\n";
  return os;
}
  
void Label::deserialize(XML_Parser *parser, ElementFactory *ef) {
  DeserializeInfo *info = new DeserializeInfo();
  info->old_data = XML_GetUserData(parser);
  info->ef = ef;
  info->parser = parser;
  info->collect_text = false;
  info->depth = 0;
  XML_SetUserData(parser, info);
  XML_SetElementHandler(parser, start_handler, end_handler);
}

u_fp32 Label::select_distance_sqr(fp16 x, fp16 y) const {
  fp16 bx, by, bw, bh;
  get_bounding_box(bx, by, bw, bh);

  // Check if inside the box
  if (x >= bx && x < bx + bw && y >= by && y < by + bh)
    return 0;
  
  // Calculate the distance to box sides
  u_fp32 min_dist = ~static_cast<u_fp32>(0);
  u_fp32 dist = distance_to_line_sqr(x, y, bx, by, bw - 1, 0);
  if (dist < min_dist)
    min_dist = dist;
  dist = distance_to_line_sqr(x, y, bx + bw - 1, by, 0, bh - 1);
  if (dist < min_dist)
    min_dist = dist;
  dist = distance_to_line_sqr(x, y, bx + bw - 1, by + bh - 1, -bw + 1, 0);
  if (dist < min_dist)
    min_dist = dist;
  dist = distance_to_line_sqr(x, y, bx, by + bh - 1, 0, -bh + 1);
  if (dist < min_dist)
    min_dist = dist;
  return min_dist;
}
  
void Label::move(fp16 xoff, fp16 yoff) {
  x += xoff;
  y += yoff;
}

const char *Label::get_text() const {
  return text.c_str();
}

void Label::set_text(const char *_text) {
  text.assign(_text);
}

size_t Label::get_text_length() const {
  return text.length();
}

void Label::insert_text(size_t pos, const char *txt) {

  // Keep the position within limits
  if (pos > text.length())
    pos = text.length();

  // Insert text
  text.insert(pos, txt);
}

void Label::remove_text(size_t pos, size_t len = 1) {
  
  // Check the range
  if (pos >= text.length())
    return;
  if (pos + len > text.length())
    len = text.length() - pos;

  // Remove the text
  text.erase(pos, len);
}

void Label::draw_text_cursor(size_t pos, fp16 origin_x, fp16 origin_y,
                             fp16 scaling, bool xorred=false) const {
  
  // Keep the position within limits
  if (pos > text.length())
    pos = text.length();

  // Calculate the cursor position and size
  fl_font(FL_HELVETICA | FL_BOLD, 16);
  int font_height = fl_height();
  int cursor_x = 0;
  int cursor_y = 0;
  if (pos > 0) {
    const char *text_c_str = text.c_str();
    char *tmp_c_str = new char[pos+1];
    memcpy(tmp_c_str, text_c_str, pos);
    tmp_c_str[pos] = '\0';
    int w=1000, h;
    fl_measure(tmp_c_str, w, h);
    delete tmp_c_str;
    cursor_x = w;
    cursor_y = h - font_height;
  }
  int cx = coord_to_screen(x + int_to_fp16(cursor_x), origin_x, scaling);
  int cy = coord_to_screen(y + int_to_fp16(cursor_y), origin_y, scaling);
  int ch = coord_to_screen(int_to_fp16(font_height), 0, scaling);
  
  // Draw the cursor (two lines)
  int old_func;
  if (xorred) {
    fl_color(FL_WHITE);
    old_func = fle_xorred_mode();
  } else
    fl_color(FL_BLACK);
  fl_line(cx, cy, cx, cy + ch - 1);
  fl_line(cx + 1, cy, cx + 1, cy + ch - 1);
  if (xorred)
    fle_reset_mode(old_func);
}
