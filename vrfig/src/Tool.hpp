// $Id: Tool.hpp,v 1.2 2001-05-08 15:31:20 jle Exp $

#ifndef __Tool_hpp_INCLUDED__
#define __Tool_hpp_INCLUDED__

#include "FigureView.hpp"

/**
 * Tools are used for editing figures. They handle the tool specific
 * user interface issues and edit the figure according to user actions.
 * Tools are typically single instance classes. This is the common base
 * class for all tools.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.2 $
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
  virtual void draw(FigureView *view) {};

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
