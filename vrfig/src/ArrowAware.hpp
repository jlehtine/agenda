// $Id: ArrowAware.hpp,v 1.1 2004-11-10 14:03:05 jle Exp $

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

#ifndef __ArrowSupport_hpp_INCLUDED__
#define __ArrowSupport_hpp_INCLUDED__

#include <vector.h>
#include "mathutil.hpp"

/**
 * Elements supporting arrow heads should implement this interface.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.1 $
 */
class ArrowAware {

public:

  /**
   * Returns the points that can have an arrow head.
   *
   * @return a vector of points that can have an arrow head
   */
  const virtual vector<ArrowPoint> *get_arrow_points() const = 0;

  /**
   * Set the arrow state of the specified arrow point.
   *
   * @param i the index of the arrow point
   * @param has_arrow whether the point should have an arrow or not
   */
  void virtual set_arrow(int i, bool has_arrow) = 0;
};

#endif
