// $Id: PolyLine.hpp,v 1.3 2001-05-08 21:20:01 jle Exp $

#ifndef __Line_hpp_INCLUDED__
#define __Line_hpp_INCLUDED__

#include <vector.h>
#include "Element.hpp"

/**
 * A polyline element which is the most generic element.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.3 $
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
  void draw(int origin_x, int origin_y, unsigned int scaling);

  // Doc inherited from Element
  static PolyLine *fit_to_points(
    vector<int> *points, int origin_x, int origin_y, unsigned int scaling);
  
};

#endif
