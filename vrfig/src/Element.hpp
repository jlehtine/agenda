// $Id: Element.hpp,v 1.10 2001-05-24 18:24:27 jle Exp $

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

#ifndef __Element_hpp_INCLUDED__
#define __Element_hpp_INCLUDED__

#include <string>
#include <vector.h>
#include <ostream.h>
#include "mathutil.hpp"

/**
 * Figure is composed of elements such as lines or circles. The elements
 * can be created and edited using the tools. Elements take care of
 * rendering themselves and each element also includes static code to
 * determine if a specific set of points could be matched by the
 * element. This is the common base class for all elements.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 */
class Element {

protected:

  /**
   * Writes an element name with an optional namespace identifier to
   * the specified output stream.
   *
   * @param os the output stream
   * @param ns the namespace identifier or null if default namespace
   * @param name the name of the element
   * @return the output stream for continuation
   */
  static ostream &output_ns_name(ostream &os, const char *ns, 
                                 const char *name) {
    if (ns)
      os << ns << ":";
    os << name;
    return os;
  }

  /**
   * Output indentation to the specified output stream.
   *
   * @param os the output stream
   * @param indent the depth of indentation
   * @return the output stream for continuation
   */
  static ostream &output_indent(ostream &os, int indent) {
    while (indent--)
      os << " ";
    return os;
  }

public:

  virtual ~Element() {}

  /**
   * Returns the name of this element (like "polyline" or "circle").
   *
   * @return the name of this element
   */
  virtual const char *get_name() const = 0;

  /**
   * Returns the XML namespace for this element. The namespace is
   * used when serializing/deserializing data to/from XML document.
   * The default implementation uses the namespace
   * 'http://www.iki.fi/jle/agenda/vrfig/' but extensions not included
   * in the standard distribution should use some other namespace. This
   * will become more relevant if/when the plugins will become supported.
   *
   * @return the namespace for this element.
   */
  virtual const char *get_namespace() const;

  /**
   * Returns the bounding box for the element.
   *
   * @param x the coordinate of the left edge of the box 
   * @param y the coordinate of the bottom edge of the box 
   * @param w the width of the box 
   * @param h the height of the box 
   */
  virtual void get_bounding_box(fp16 &x, fp16 &y, fp16 &w, fp16 &h) 
    const = 0;

  /**
   * Draws the element to the current widget using the specified scaling
   * and origin.
   *
   * @param origin_x the x coordinate for the origin 
   * @param origin_y the y coordinate for the origin 
   * @param scaling the scaling used 
   * @param xor whether the element should be drawn xorred
   */
  virtual void draw(fp16 origin_x, fp16 origin_y, u_fp16 scaling,
                    bool xorred=false) const = 0;
  
  /**
   * Serializes element data to the specified ostream. The data is serialized
   * as an XML data using UTF-8 charset. The caller creates an enclosing
   * XML element so the serialized data does not need to be within one
   * element.
   *
   * @param os the output stream for element data
   * @param ns the namespace identifier for the element namespace or \
   *        null if the default namespace should be used
   * @param indent the base indentation level
   * @return the output stream for continuation
   */
  virtual ostream &serialize(ostream &os, const char *ns, int indent) 
    const = 0;

#if USE_EXPERIMENTAL_UI
  /**
   * Fits some element to the specified point set or returns
   * null if no element could be fitted.
   *
   * @param points a vector of point x/y coordinates
   * @param origin_x the x coordinate for the origin 
   * @param origin_y the y coordinate for the origin 
   * @param scaling the scaling used 
   * @return the fitted element
   */
  static Element *fit_element(
    vector<int> *points, fp16 origin_x, fp16 origin_y, u_fp16 scaling);
#endif
};

#endif
