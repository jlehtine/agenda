// $Id: PolyLine.hpp,v 1.4 2001-05-16 19:50:16 jle Exp $

#ifndef __Line_hpp_INCLUDED__
#define __Line_hpp_INCLUDED__

#include <vector.h>
#include "Element.hpp"
#include "vrfig.hpp"

/**
 * A polyline element which is the most generic element.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.4 $
 */
class PolyLine : public Element {

protected:

  /** Polyline vertices */
  vector<int> points;

  /** Whether this is actually a polygon */
  bool closed;

  inline PolyLine(): points() {}

public:

  virtual ~PolyLine() {
    points.clear();
  }

  // Doc inherited from Element
  inline const string *get_name() const {
    static const string name("polyline");
    return &name;
  }

  // Doc inherited from Element
  inline const string *get_namespace() const {
    return &vrf_default_namespace;
  }

  // Doc inherited from Element
  void get_bounding_box(int &x, int &y, int &w, int &h) const;

  // Doc inherited from Element
  void draw(int origin_x, int origin_y, unsigned int scaling, 
            bool xorred=false) const;

  // Doc inherited from Element
  ostream &serialize(ostream &os, const string *ns, int indent) const;

#if USE_EXPERIMENTAL_UI
  // Doc inherited from Element
  static PolyLine *fit_to_points(
    vector<int> *points, int origin_x, int origin_y, unsigned int scaling);
#endif
};

#endif
