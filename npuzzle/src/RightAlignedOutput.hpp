// $Id: RightAlignedOutput.hpp,v 1.1 2000-10-29 17:20:50 jle Exp $

#ifndef __RightAlignedOutput_hpp_INCLUDED__
#define __RightAlignedOutput_hpp_INCLUDED__

#include <FL/Fl_Output.H>

#ifdef __cplusplus

/**
 * Like Fl_Output but draws content right aligned.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.1 $
 */
class RightAlignedOutput : public Fl_Output {

public:

  inline RightAlignedOutput(int x, int y, int w, int h, const char *l = 0)
    : Fl_Output(x, y, w, h, l) {}

  /**
   * Overrides draw in Fl_Output.
   */
  virtual void draw();
};

#endif
#endif
