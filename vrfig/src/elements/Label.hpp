// $Id: Label.hpp,v 1.2 2001-06-10 18:36:43 jle Exp $

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

#ifndef __Label_hpp_INCLUDED__
#define __Label_hpp_INCLUDED__

#include <string>
#include <expat.h>
#include "Element.hpp"
#include "Selectable.hpp"
#include "Movable.hpp"
#include "Textual.hpp"
#include "ElementFactory.hpp"
#include "Point.hpp"

/**
 * A text label element.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.2 $
 */
class Label : public Element, public Selectable, public Movable,
              public Textual {

protected:

  /** The label position */
  Point position;

  /** The label text */
  string text;

public:

  /**
   * Constructs a new label.
   *
   * @param x the x coordinate
   * @param y the y coordinate
   * @param text the text for the label (buffer is copied)
   */
  inline Label(fp16 x, fp16 y, const char *text):
    position(x, y), text(text) {}

  static const char *get_name_static();
  virtual const char *get_name() const;
  
  static const char *get_namespace_static();
  
  virtual void get_bounding_box(fp16 &x, fp16 &y, fp16 &w, fp16 &h) 
    const;
  
  virtual void draw(fp16 origin_x, fp16 origin_y, u_fp16 scaling,
                    bool xorred=false) const;
  
  virtual ostream &serialize(ostream &os, const char *ns, int indent) 
    const;
  
  static void deserialize(XML_Parser *parser, ElementFactory *ef);

  virtual u_fp32 select_distance_sqr(fp16 x, fp16 y) const;
  
  virtual void move(fp16 xoff, fp16 yoff);

  virtual const char *get_text() const;

  virtual void set_text(const char *text);

  virtual size_t get_text_length() const;

  virtual void insert_text(size_t pos, const char *txt);

  virtual void remove_text(size_t pos, size_t len = 1);

  virtual void draw_text_cursor(size_t pos, fp16 origin_x, fp16 origin_y,
                                fp16 scaling, bool xorred=false) const;
};

#endif
