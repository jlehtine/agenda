// $Id: RectangleTool.hpp,v 1.1 2001-05-22 21:08:10 jle Exp $

#ifndef __RectangleTool_hpp_INCLUDED__
#define __RectangleTool_hpp_INCLUDED__

#include "Tool.hpp"

/**
 * A tool for drawing rectangles.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.1 $
 */
class RectangleTool : public Tool {

protected:

  /** Whether current drawing a rectangle */
  bool drawing;

  /** The starting x screen coordinate */
  int xs;

  /** The starting y screen coordinate */
  int ys;

  /** The last x screen coordinate */
  int last_x;
  
  /** The last y screen coordinate */
  int last_y;

public:

  /** Constructs a new RectangleTool */
  inline RectangleTool(): drawing(false) {}

  virtual const char *get_name() const;

  virtual void draw_icon(int x, int y, int w, int h) const;

  virtual void deactivated(FigureView *view);

  virtual void draw(FigureView *view);

  virtual int handle(int event, FigureView *view);
  
};

#endif
