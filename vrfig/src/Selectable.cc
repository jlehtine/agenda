// $Id: Selectable.cc,v 1.1 2001-05-19 16:54:22 jle Exp $

#include <FL/fl_draw.H>
#include <FL/Enumerations.H>
#include "Selectable.hpp"

#define HELPER_RECT_SIZE 5

void Selectable::draw_select_helper(int x, int y) {
  fl_rect(x - HELPER_RECT_SIZE/2, y - HELPER_RECT_SIZE/2,
          HELPER_RECT_SIZE, HELPER_RECT_SIZE);
}

void Selectable::draw_select_helper(
  fp16 x, fp16 y, fp16 origin_x, fp16 origin_y, u_fp16 scaling) {
  draw_select_helper(coord_to_screen(x, origin_x, scaling),
                     coord_to_screen(y, origin_y, scaling));
}
