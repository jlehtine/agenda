// $Id: Selectable.cc,v 1.2 2001-05-23 12:47:50 jle Exp $

/*--------------------------------------------------------------------------
 * VRFig, a vector graphics editor for PDA environment
 * Copyright 2001 Johannes Lehtinen
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *------------------------------------------------------------------------*/

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
