// $Id: PolyLine.hpp,v 1.3 2001-05-19 16:56:26 jle Exp $

#ifndef __Line_hpp_INCLUDED__
#define __Line_hpp_INCLUDED__

#include <vector.h>
#include "Element.hpp"
#include "Selectable.hpp"
#include "Movable.hpp"
#include "vrfig.hpp"
#include "mathutil.hpp"

/**
 * A polyline element which is the most generic element.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.3 $
 */
class PolyLine : public Element, public Selectable, public Movable {

protected:

  /** Polyline vertices */
  vector<fp16> points;

  /** Whether this is actually a polygon */
  bool closed;

public:

  inline PolyLine(): points() {}

  ~PolyLine();

  virtual const string *get_name() const;

  virtual const string *get_namespace() const;

  virtual void get_bounding_box(fp16 &x, fp16 &y, fp16 &w, fp16 &h) const;

  virtual void draw(fp16 origin_x, fp16 origin_y, u_fp16 scaling, 
                    bool xorred=false) const;

  virtual ostream &serialize(ostream &os, const string *ns, int indent) const;

  virtual void draw_select_helpers(
    int origin_x, int origin_y, unsigned int scaling, bool xorred=false) const;

  virtual fp16 select_distance(fp16 x, fp16 y) const;

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
