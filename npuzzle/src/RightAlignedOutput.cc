// $Id: RightAlignedOutput.cc,v 1.1 2000-10-29 17:20:50 jle Exp $

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
