// $Id: EditTool.hpp,v 1.1 2001-05-22 19:50:23 jle Exp $

#ifndef __EditTool_hpp_INCLUDED__
#define __EditTool_hpp_INCLUDED__

#include "Tool.hpp"
#include "Controllable.hpp"

/**
 * Tool for controlling the existing elements.
 *
 * @author Johannes Lehtinen <johannes.lehtine@iki.fi>
 * @version $Revision: 1.1 $
 */
class EditTool : public Tool {

protected:

  /** The element being currently controlled or null if none */
  Controllable *element;

  /** Index of the control point being controlled */
  int cp_index;

  /** The original x coordinate for the control point */
  fp16 org_x;

  /** The original y coordinate for the control point */
  fp16 org_y;

  /** The screen x coordinate of the starting point */
  int sx;

  /** The screen y coordinate of the starting point */
  int sy;

public:

  /** Constructs a new EditTool */
  inline EditTool(): element(0) {}

  virtual const char *get_name() const;

  virtual void draw_icon(int x, int y, int w, int h) const;

  virtual void deactivated(FigureView *view);

  virtual void draw(FigureView *view);

  virtual int handle(int event, FigureView *view);
};

#endif
