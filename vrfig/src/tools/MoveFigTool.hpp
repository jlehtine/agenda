// $Id: MoveFigTool.hpp,v 1.2 2001-05-20 23:42:14 jle Exp $

#ifndef __MoveFigTool_hpp_INCLUDED__
#define __MoveFigTool_hpp_INCLUDED__

#include "Tool.hpp"

/**
 * The MoveFigTool is used to move the figure around the view.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.2 $
 */
class MoveFigTool : public Tool {

protected:

  /** Whether currently moving the figure */
  bool moving;

  /** The original screen coordinates */
  int screen_coord[2];

  /** The original figure origin coordinates */
  int fig_coord[2];

public:

  inline MoveFigTool(): moving(false) {}

  virtual const char *get_name() const;
  
  void draw_icon(int x, int y, int w, int h) const;

  void deactivated(FigureView *view);
  
  int handle(int event, FigureView *view);
};

#endif
