// $Id: MoveFigTool.hpp,v 1.1 2001-05-08 15:31:20 jle Exp $

#ifndef __MoveFigTool_hpp_INCLUDED__
#define __MoveFigTool_hpp_INCLUDED__

#include "Tool.hpp"

/**
 * The MoveFigTool is used to move the figure around the view.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.1 $
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

  inline void deactivated(FigureView *view) {
    moving = false;
  }

  // Doc inherited from Tool
  int handle(int event, FigureView *view);
};

#endif
