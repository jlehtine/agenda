// $Id: flext.hpp,v 1.1 2001-05-16 19:50:16 jle Exp $

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
