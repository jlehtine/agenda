// $Id: PolyLine.hpp,v 1.6 2001-05-22 18:03:59 jle Exp $

#ifndef __Line_hpp_INCLUDED__
#define __Line_hpp_INCLUDED__

#include <vector.h>
#include <expat.h>
#include "Element.hpp"
#include "Selectable.hpp"
#include "Movable.hpp"
#include "vrfig.hpp"
#include "mathutil.hpp"
#include "ElementFactory.hpp"

/**
 * A polyline element which is the most generic element.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.6 $
 */
class PolyLine : public Element, public Selectable, public Movable {

protected:

  /** Polyline vertices */
  vector<fp16> points;

  /** Whether this is actually a polygon */
  bool closed;

public:

  inline PolyLine(): points(), closed(false) {}

  ~PolyLine();

  static const char *get_name_static();
  virtual const char *get_name() const;

  static const char *get_namespace_static();
  virtual const char *get_namespace() const;

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

  /**
   * Returns the vertices of the polyline.
   *
   * @return a vector with the vertices of the polyline
   */
  inline vector<fp16> *get_vertices() {
    return &points;
  }

  /**
   * Returns the vertices of the polyline for reading only.
   *
   * @return a vector with the vertices of the polyline
   */
  inline const vector<fp16> *get_vertices() const {
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
