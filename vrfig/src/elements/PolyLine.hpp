// $Id: PolyLine.hpp,v 1.10 2001-05-29 18:05:10 jle Exp $

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

#ifndef __Line_hpp_INCLUDED__
#define __Line_hpp_INCLUDED__

#include <vector.h>
#include <expat.h>
#include "Element.hpp"
#include "Selectable.hpp"
#include "Movable.hpp"
#include "Controllable.hpp"
#include "vrfig.hpp"
#include "mathutil.hpp"
#include "ElementFactory.hpp"

/**
 * A polyline element which is the most generic element.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.10 $
 */
class PolyLine : public Element, public Selectable, public Movable,
  public Controllable {

protected:

  /** Polyline vertices */
  vector<Point> points;

  /** Whether this is actually a polygon */
  bool closed;

public:

  inline PolyLine(): points(), closed(false) {}

  ~PolyLine();

  static const char *get_name_static();
  virtual const char *get_name() const;

  static const char *get_namespace_static();

  virtual void get_bounding_box(fp16 &x, fp16 &y, fp16 &w, fp16 &h) const;

  virtual void draw(fp16 origin_x, fp16 origin_y, u_fp16 scaling, 
                    bool xorred=false) const;

  virtual ostream &serialize(ostream &os, const char *ns, int indent) const;

  static void deserialize(XML_Parser *parser, ElementFactory *ef);

#if 0
  virtual void draw_select_helpers(
    int origin_x, int origin_y, unsigned int scaling, bool xorred=false) const;
#endif

  virtual u_fp32 select_distance_sqr(fp16 x, fp16 y) const;

  virtual void move(fp16 xoff, fp16 yoff);

  virtual const vector<Point> *get_control_points() const;
  
  virtual void control(unsigned int i, fp16 x, fp16 y);

  /**
   * Returns the vertices of the polyline.
   *
   * @return a vector with the vertices of the polyline
   */
  inline vector<Point> *get_vertices() {
    return &points;
  }

  /**
   * Returns the vertices of the polyline for reading only.
   *
   * @return a vector with the vertices of the polyline
   */
  inline const vector<Point> *get_vertices() const {
    return &points;
  }

  /**
   * Check if this polyline is actually a polygon.
   *
   * @return whether the polyline is closed (polygon)
   */
  inline bool is_closed() const {
    return closed;
  }

  /**
   * Set the closed flag for this polyline.
   *
   * @param c whether the polyline should be closed (polygon)
   */
  inline void set_closed(bool c) {
    closed = c;
  }
  
#if USE_EXPERIMENTAL_UI
  static PolyLine *fit_to_points(
    vector<int> *points, fp16 origin_x, fp16 origin_y, u_fp16 scaling);
#endif
};

#endif
