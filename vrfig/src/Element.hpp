// $Id: Element.hpp,v 1.1 2001-05-06 22:12:08 jle Exp $

/**
 * Each figure is composed of elements such as lines or circles. The elements
 * can be created and edited using the editor view. This is the common
 * base class for all elements.
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
   * @param orig_x the x coordinate of the lower left corner
   * @param orix_y the y coordinate of the lower left corner
   * @param scale the scaling used as 16.16 fixed point value
   */
  virtual void draw(int orig_x, int orig_y, unsigned int scale) = 0;
};
