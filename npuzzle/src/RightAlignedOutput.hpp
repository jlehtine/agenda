// $Id: RightAlignedOutput.hpp,v 1.2 2000-10-31 19:52:42 jle Exp $

/*
* NPuzzle
* Copyright (C) 2000 Johannes Lehtinen
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* The license can be found in the package directory in file COPYING.
*/

#ifndef __RightAlignedOutput_hpp_INCLUDED__
#define __RightAlignedOutput_hpp_INCLUDED__

#include <FL/Fl_Output.H>

#ifdef __cplusplus

/**
 * Like Fl_Output but draws content right aligned.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.2 $
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
