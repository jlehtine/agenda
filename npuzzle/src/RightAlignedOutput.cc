// $Id: RightAlignedOutput.cc,v 1.2 2000-10-31 19:52:42 jle Exp $

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

#include <FL/fl_draw.H>
#include "RightAlignedOutput.hpp"

void RightAlignedOutput::draw() {

  // Draw box
  draw_box();
  
  // Draw text
  fl_color(textcolor());
  fl_font(textfont(), textsize());
  fl_draw(value(), x() + 4, y() + 4, w() - 8, h() - 8, FL_ALIGN_RIGHT);
}
