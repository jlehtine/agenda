// $Id: Element.hpp,v 1.3 2001-05-08 15:31:19 jle Exp $

#ifndef __Element_hpp_INCLUDED__
#define __Element_hpp_INCLUDED__

#include <vector.h>

/**
 * Figure is composed of elements such as lines or circles. The elements
 * can be created and edited using the tools. Elements take care of
 * rendering themselves and each element also includes static code to
 * determine if a specific set of points could be matched by the
 * element. This is the common base class for all elements.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 */
class Element {

public:

  /**
   * Returns the bounding box for the element. All the visual parts of the
   * element should fit inside the bounding box including control points.
   *
   * @param x the coordinate of the left edge of the box
   * @param y the coordinate of the bottom edge of the box
   * @param w the width of the box
   * @param h the height of the box
   */
  //virtual void get_bounding_box(int *x, int *y, int *w, int *h) = 0;
  
  /**
   * Draws the element to the current widget using the specified scaling
   * and origin.
   *
   * @param origin_x the x coordinate for the origin (16.16 fixed)
   * @param origin_y the y coordinate for the origin (16.16 fixed)
   * @param scaling the scaling used (16.16 fixed)
   */
  virtual void draw(int origin_x, int origin_y, unsigned int scaling) = 0;

  /**
   * Fits some element to the specified point set or returns
   * null if no element could be fitted.
   *
   * @param points a vector of point x/y coordinates
   * @param origin_x the x coordinate for the origin (16.16 fixed)
   * @param origin_y the y coordinate for the origin (16.16 fixed)
   * @param scaling the scaling used (16.16 fixed)
   * @return the fitted element
   */
  static Element *fit_element(
    vector<int> *points, int origin_x, int origin_y, unsigned int scaling);
};

#endif
