// $Id: Figure.hpp,v 1.5 2001-05-23 12:47:50 jle Exp $

/*--------------------------------------------------------------------------
 * VRFig, a vector graphics editor for PDA environment
 * Copyright 2001 Johannes Lehtinen
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *------------------------------------------------------------------------*/

#ifndef __Figure_hpp_INCLUDED__
#define __Figure_hpp_INCLUDED__

#include <ostream.h>
#include <vector.h>
#include "Element.hpp"

/**
 * Represents the whole figure composed of elements.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.5 $
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

  ~Figure();

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
