// $Id: Controllable.hpp,v 1.1 2001-05-22 19:49:25 jle Exp $

#ifndef __Controllable_hpp_INCLUDED__
#define __Controllable_hpp_INCLUDED__

#include "FigureView.hpp"
#include "Selectable.hpp"
#include "mathutil.hpp"

/**
 * An interface for elements that can be controlled by control points.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.1 $
 */
class Controllable {

public:

  /**
   * Returns a list of control points as x/y pairs.
   *
   * @return a list of control points
   */
  virtual const vector<fp16> *get_control_points() const = 0;

  /**
   * Moves the specified control point to the specified location.
   * Because of element specific restrictions the control point might
   * not move exactly to the point pointed by the user. The control
   * points should be read after the move operation to find out where
   * the point actually moved.
   *
   * @param i the index of control point to move
   * @param x the new x coordinate for the control point
   * @param y the new y coordinate for the control point
   */
  virtual void control(unsigned int i, fp16 x, fp16 y) = 0;
};

#endif
