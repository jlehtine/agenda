// $Id: Rectangle.hpp,v 1.5 2001-05-29 18:05:11 jle Exp $

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

#ifndef __Rectangle_hpp_INCLUDED__
#define __Rectangle_hpp_INCLUDED__

#include <expat.h>
#include "Element.hpp"
#include "Movable.hpp"
#include "Selectable.hpp"
#include "Controllable.hpp"
#include "ElementFactory.hpp"

class Rectangle : public Element, public Selectable, public Movable,
                  public Controllable {

protected:

  /** The x coordinate of the lower left corner */
  fp16 x;

  /** The y coordinate of the lower left corner */
  fp16 y;

  /** The width */
  fp16 w;
  
  /** The height */
  fp16 h;
  
  /** The control points (redundant information updated based on x, y, w, h) */
  vector<Point> controls;

  /**
   * Updates the control points based on the current x, y, w, h values.
   */
  void update_controls();

public:

  /**
   * Constructs a new rectangle.
   *
   * @param x the x coordinate of the lower left corner
   * @param y the y coordinate of the lower left corner
   * @param w the width of the rectangle
   * @param h the height of the rectangle
   */
  inline Rectangle(fp16 x, fp16 y, fp16 w, fp16 h): 
    x(x), y(y), w(w), h(h), controls(4) {
    update_controls();
  }

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
  
  virtual const vector<Point> *get_control_points() const;

  virtual void control(unsigned int i, fp16 x, fp16 y);
  
};

#endif
