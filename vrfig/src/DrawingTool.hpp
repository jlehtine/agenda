// $Id: DrawingTool.hpp,v 1.2 2001-05-07 21:02:10 jle Exp $

#ifndef __DrawingTool_hpp_INCLUDED__
#define __DrawingTool_hpp_INCLUDED__

#include <vector>
#include "Tool.hpp"

/**
 * A common base class for tools that draw new elements.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.2 $
 */
class DrawingTool : public Tool {

protected:

  /** The recorded points */
  vector<int> points;

  /** Whether we are currently drawing object */
  bool drawing;
  
  /**
   * Draws the current line as xorred line.
   *
   * @param view the view to use for drawing
   */
  void draw_current_line(FigureView *view);

public:

  /**
   * Constructs a new tool.
   */
  inline DrawingTool(): drawing(false), points() {}

  // Doc inherited from Tool
  void deactivated(FigureView *view);

  /**
   * If the user was drawing, draw the recorded points. Otherwise
   * do nothing.
   *
   * @param view the current view
   */
  virtual void draw(FigureView *view);
  
  // Doc inherited from Tool
  virtual int handle(int event, FigureView *view);
};

#endif
