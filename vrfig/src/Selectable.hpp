// $Id: Selectable.hpp,v 1.1 2001-05-19 06:17:43 jle Exp $

#ifndef __Selectable_hpp_INCLUDED__
#define __Selectable_hpp_INCLUDED__

/**
 * An interface for elements that may be selected. Implementing this
 * interface is required by many tools including delete, move and copy.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.1 $
 */
class Selectable {

public:

  /**
   * Draws possible extra visual cues for making the selection operation
   * easier.
   *
   * @param origin_x the x coordinate for the origin (16.16 fixed)
   * @param origin_y the y coordinate for the origin (16.16 fixed)
   * @param scaling the scaling used (16.16 fixed)
   * @param xor whether the element should be drawn xorred
   */
  virtual void draw_select_helpers(
    int origin_x, int origin_y, unsigned int scaling, bool xorred=false) {}

  /**
   * Calculates the distance between the selection point and the element.
   * This method is called for those elements whose bounding boxes are
   * close enough to the selected point. The returned distance is used
   * to find the closest element.
   *
   * @param x the x coordinate of selection point (16.16 fixed)
   * @param y the y coordinate of selection point (16.16 fixed)
   * @return the distance to the point (16.16 fixed)
   */
  virtual int select_distance(int x, int y) = 0;
  
};

#endif
