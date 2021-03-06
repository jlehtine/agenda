// $Id: flext.hpp,v 1.2 2001-05-23 12:47:51 jle Exp $

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

#ifndef __flext_hpp_INCLUDED__
#define __flext_hpp_INCLUDED__

/**
 * Sets xorred drawing mode.
 *
 * @return the old drawing function to be passed for fle_reset_mode()
 */
int fle_xorred_mode();

/**
 * Resets the drawing mode.
 *
 * @param old_func the old function returned by fle_xorred_mode()
 */
void fle_reset_mode(int old_func);

/**
 * Draw 1-pixel xorred point.
 *
 * @param x the x coordinate of the point
 * @param y the y coordinate of the point
 */
void fle_xorred_point(int x, int y);

/**
 * Draw 1-pixel xorred line between the specified points.
 *
 * @param x the x coordinate of the first point
 * @param y the y coordinate of the first point
 * @param x1 the x coordinate of the second point
 * @param y1 the y coordinate of the second point
 */
void fle_xorred_line(int x, int y, int x1, int y1);

#endif
