// $Id: MoveTool.hpp,v 1.1 2001-05-22 18:04:28 jle Exp $

#ifndef __MoveTool_hpp_INCLUDED__
#define __MoveTool_hpp_INCLUDED__

#include "Tool.hpp"
#include "Movable.hpp"
#include "Selectable.hpp"
#include "Figure.hpp"

/**
 * This tool can move elements which implement Selectable and Movable
 * interfaces.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.1 $
 */
class MoveTool : public Tool {
  
protected:

  /** The element currently being moved or null if none */
  Movable *element;
  
  /** The last x coordinate for the mouse */
  int last_x;

  /** The last y coordinate for the mouse */
  int last_y;

public:

  MoveTool(): element(0) {}

  virtual const char *get_name() const;

  virtual void draw_icon(int x, int y, int w, int h) const;

  virtual void deactivated(FigureView *view);

  virtual void draw(FigureView *view);

  virtual int handle(int event, FigureView *view);
};

#endif
