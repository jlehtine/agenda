// $Id: PolyLine.hpp,v 1.1 2001-05-17 19:35:28 jle Exp $

#ifndef __Line_hpp_INCLUDED__
#define __Line_hpp_INCLUDED__

#include <vector.h>
#include "Element.hpp"
#include "vrfig.hpp"

/**
 * A polyline element which is the most generic element.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.1 $
 */
class PolyLine : public Element {

protected:

  /** Polyline vertices */
  vector<int> points;

  /** Whether this is actually a polygon */
  bool closed;

public:

  inline PolyLine(): points() {}

  ~PolyLine();

  const string *get_name() const;

  const string *get_namespace() const;

  void get_bounding_box(int &x, int &y, int &w, int &h) const;

  void draw(int origin_x, int origin_y, unsigned int scaling, 
            bool xorred=false) const;

  ostream &serialize(ostream &os, const string *ns, int indent) const;

#if USE_EXPERIMENTAL_UI
  static PolyLine *fit_to_points(
    vector<int> *points, int origin_x, int origin_y, unsigned int scaling);
#endif
};

#endif
