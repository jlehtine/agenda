// $Id: PolyLine.hpp,v 1.1 2001-05-07 21:02:10 jle Exp $

#ifndef __Line_hpp_INCLUDED__
#define __Line_hpp_INCLUDED__

#include <vector.h>
#include "Element.hpp"

/**
 * A polyline element which is the most generic element.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.1 $
 */
class PolyLine : public Element {

protected:

  vector<int> points;

  inline PolyLine(): points() {}

public:

  // Doc inherited from Element
  void draw(int origin_x, int origin_y, unsigned int scaling);

  // Doc inherited from Element
  static PolyLine *fit_to_points(
    vector<int> *points, int origin_x, int origin_y, unsigned int scaling);
  
};

#endif
