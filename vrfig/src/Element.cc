// $Id: Element.cc,v 1.2 2001-05-16 19:50:16 jle Exp $

#include "Element.hpp"

#if USE_EXPERIMENTAL_UI
#include "PolyLine.hpp"
#endif

#if USE_EXPERIMENTAL_UI
Element *Element::fit_element(
  vector<int> *points, int origin_x, int origin_y, unsigned int scaling) {
  
  // Try more specific elements first
  Element *element = PolyLine::fit_to_points
    (points, origin_x, origin_y, scaling);
  if (element != 0)
    return element;
  return 0;
}
#endif
