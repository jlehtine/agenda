// $Id: EllipseTool.hpp,v 1.1 2001-05-23 07:55:00 jle Exp $

#ifndef __EllipseTool_hpp_INCLUDED__
#define __EllipseTool_hpp_INCLUDED__

#include "Tool.hpp"
#include "FigureView.hpp"
#include "elements/Ellipse.hpp"

/**
 * A tool for drawing ellipses.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.1 $
 */
class EllipseTool : public Tool {

protected:

  /** The ellipse being currently drawn or null if none */
  Ellipse *ellipse;

public:

  /** Construct a new tool */
  inline EllipseTool(): ellipse(0) {}

  virtual const char *get_name() const;

  virtual void draw_icon(int x, int y, int w, int h) const;

  virtual void deactivated(FigureView *view);

  virtual void draw(FigureView *view);

  virtual int handle(int event, FigureView *view);
  
};

#endif
