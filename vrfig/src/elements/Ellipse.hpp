// $Id: Ellipse.hpp,v 1.2 2001-05-23 12:47:51 jle Exp $

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

#ifndef __Ellipse_hpp_INCLUDED__
#define __Ellipse_hpp_INCLUDED__

#include <expat.h>
#include "Element.hpp"
#include "ElementFactory.hpp"
#include "Selectable.hpp"
#include "Movable.hpp"
#include "Controllable.hpp"

/**
 * An ellipse element.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.2 $
 */
class Ellipse : public Element, public Selectable, public Movable,
                public Controllable {

protected:

  /** The control points, including first the center and the width and height*/
  vector<fp16> controls;

public:

  /**
   * Constructs a new ellipse.
   */
  Ellipse(fp16 x, fp16 y, fp16 x1, fp16 y1);

  static const char *get_name_static();
  virtual const char *get_name() const;
  
  static const char *get_namespace_static();
  virtual const char *get_namespace() const;
  
  virtual void get_bounding_box(fp16 &x, fp16 &y, fp16 &w, fp16 &h) 
    const;
  
  virtual void draw(fp16 origin_x, fp16 origin_y, u_fp16 scaling,
                    bool xorred=false) const;
  
  virtual ostream &serialize(ostream &os, const char *ns, int indent) 
    const;
  
  static void deserialize(XML_Parser *parser, ElementFactory *ef);

  virtual u_fp32 select_distance_sqr(fp16 x, fp16 y) const;
  
  virtual void move(fp16 xoff, fp16 yoff);
  
  virtual const vector<fp16> *get_control_points() const;

  virtual void control(unsigned int i, fp16 x, fp16 y);
  
};

#endif
