// $Id: Figure.hpp,v 1.2 2001-05-07 21:02:10 jle Exp $

#ifndef __Figure_hpp_INCLUDED__
#define __Figure_hpp_INCLUDED__

#include <vector.h>
#include "Element.hpp"

/**
 * Represents the whole figure composed of elements.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.2 $
 */
class Figure {

protected:

  /** The elements in the figure */
  vector<Element *> elements;

public:

  Figure(): elements() {}

  inline vector<Element *> *get_elements() { 
    return &elements; 
  }

  inline void add_element(Element *element) {
    elements.insert(elements.end(), element);
  }
};

#endif
