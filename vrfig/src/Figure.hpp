// $Id: Figure.hpp,v 1.3 2001-05-16 19:50:16 jle Exp $

#ifndef __Figure_hpp_INCLUDED__
#define __Figure_hpp_INCLUDED__

#include <ostream.h>
#include <vector.h>
#include "Element.hpp"

/**
 * Represents the whole figure composed of elements.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.3 $
 */
class Figure {

protected:

  /** The elements in the figure */
  vector<Element *> elements;

public:

  /**
   * Constructs a new empty figure.
   */
  Figure(): elements() {}

  /**
   * Returns a vector of elements in this figure.
   *
   * @return the elements of this figure
   */
  inline vector<Element *> *get_elements() { 
    return &elements; 
  }

  /**
   * Adds a new element to the figure.
   *
   * @param element the element to be added
   */
  inline void add_element(Element *element) {
    elements.insert(elements.end(), element);
  }

  /**
   * Serializes the figure to the specified output stream. The figure
   * is serialized as an XML document.
   *
   * @param os the output stream for serialized data
   */
  void serialize(ostream &os);
};

#endif
