// $Id: DeleteTool.hpp,v 1.2 2001-05-22 19:49:12 jle Exp $

#ifndef __DeleteTool_hpp_INCLUDED__
#define __DeleteTool_hpp_INCLUDED__

#include "Tool.hpp"

/**
 * Tool for deleting elements.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.2 $
 */
class DeleteTool : public Tool {
  
public:

  /** Constructs a new DeleteTool */
  inline DeleteTool() {}

  virtual const char *get_name() const;

  virtual void draw_icon(int x, int y, int w, int h) const;

  virtual void draw(FigureView *view);

  virtual int handle(int event, FigureView *view);
};

#endif
