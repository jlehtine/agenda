// $Id: Tool.hpp,v 1.1 2001-05-06 22:12:08 jle Exp $

#ifndef __Tool_hpp_INCLUDED__
#define __Tool_hpp_INCLUDED__

#include "FigureView.hpp"

/**
 * Tools are used for editing figures. They also take care of
 * visualizing the edit actions. This is the common base class for all
 * tools.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.1 $
 */
class Tool {

public:

  /**
   * Tells that the tool has been activated.
   *
   * @param view the current state of the view
   */
  virtual void activated(FigureView *view) {}

  /**
   * Tells that the tool has been deactivated.
   *
   * @param view the current state of the view
   */
  virtual void deactivated(FigureView *view) {}

  /**
   * Draws the current state of edit action to the current widget. The
   * figure and possible control points have already been drawn.
   *
   * @param view the current state of the view
   */
  virtual void draw(FigureView *view) = 0;

  /**
   * Handles the specified event.
   *
   * @param event the event to handle
   * @param view the current state of the view
   * @return whether tool processed the event or not
   */
  virtual int handle(int event, FigureView *view) = 0;
};

#endif
