// $Id: Element.cc,v 1.4 2001-05-23 12:47:50 jle Exp $

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
