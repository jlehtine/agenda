// $Id: Selectable.hpp,v 1.2 2001-05-19 16:54:22 jle Exp $

#ifndef __Selectable_hpp_INCLUDED__
#define __Selectable_hpp_INCLUDED__

#include "FigureView.hpp"
#include "mathutil.hpp"

/**
 * An interface for elements that may be selected. Implementing this
 * interface is required by many tools including delete, move and copy.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.2 $
 */
class Selectable {

public:

  /** 
   * Draws a select helper rectangle to the specified point on the
   * screen. The pen color and possible XOR mode must be preset.
   *
   * @param x the x coordinate on the screen
   * @param y the y coordinate on the screen
   */
  static void draw_select_helper(int x, int y);

  /**
   * Draws a select helper rectangle to the specified point. The pen color
   * and possible XOR mode must be preset.
   *
   * @param x the x coordinate
   * @param y the y coordinate
   * @param view the current view
   */
  static void draw_select_helper(fp16 x, fp16 y, fp16 origin_x, fp16 origin_y,
                                 u_fp16 scaling);

  /**
   * Draws possible extra visual cues for making the selection operation
   * easier.
   *
   * @param origin_x the x coordinate for the origin
   * @param origin_y the y coordinate for the origin
   * @param scaling the scaling used
   * @param xor whether the element should be drawn xorred
   */
  virtual void draw_select_helpers(
    fp16 origin_x, fp16 origin_y, u_fp16 scaling, bool xorred=false) 
    const {}

  /**
   * Calculates the distance between the selection point and the element.
   * This method is called for those elements whose bounding boxes are
   * close enough to the selected point. The returned distance is used
   * to find the closest element.
   *
   * @param x the x coordinate of selection point
   * @param y the y coordinate of selection point
   * @return the distance to the point
   */
  virtual fp16 select_distance(fp16 x, fp16 y) const = 0;
  
};

#endif
