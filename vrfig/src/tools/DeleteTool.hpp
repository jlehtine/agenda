// $Id: DeleteTool.hpp,v 1.1 2001-05-22 18:35:26 jle Exp $

#ifndef __DeleteTool_hpp_INCLUDED__
#define __DeleteTool_hpp_INCLUDED__

#include "Tool.hpp"

class DeleteTool : public Tool {
  
public:

  /** Constructs a new DeleteTool */
  inline DeleteTool() {}

  virtual const char *get_name() const;

  virtual void draw_icon(int x, int y, int w, int h) const;

  virtual int handle(int event, FigureView *view);
};

#endif
