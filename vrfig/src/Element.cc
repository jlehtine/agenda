// $Id: Element.cc,v 1.1 2001-05-07 21:02:10 jle Exp $

#include "Element.hpp"
#include "PolyLine.hpp"

Element *Element::fit_element(
  vector<int> *points, int origin_x, int origin_y, unsigned int scaling) {
  
  // Try more specific elements first
  Element *element = PolyLine::fit_to_points
    (points, origin_x, origin_y, scaling);
  if (element != 0)
    return element;
  return 0;
}
