// $Id: Element.cc,v 1.3 2001-05-19 08:42:38 jle Exp $

#include "Element.hpp"

#if USE_EXPERIMENTAL_UI
#include "PolyLine.hpp"
#endif

#if USE_EXPERIMENTAL_UI
Element *Element::fit_element(
  vector<int> *points, fp16 origin_x, fp16 origin_y, u_fp16 scaling) {
  
  // Try more specific elements first
  Element *element = PolyLine::fit_to_points
    (points, origin_x, origin_y, scaling);
  if (element != 0)
    return element;
  return 0;
}
#endif
